#include "GlobalUtil.h"

#include <libcurl/curl/curl.h>
#include <libcurl/curl/easy.h>

glm::mat4 MathUtil::ComputeModelMatrix(Geometry::Transform3D transform)
{
    glm::mat4 model(1.0f);
    model = glm::translate(model, transform.position);
    //TODO: Rotations are euler angles for now so gimble lock + not sure about the tri-rotation
    model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, transform.scale);
    return model;
}

const std::string Curl::GetRemoteImage(const std::string& url)
{
    std::string data;

    CURL* curl = curl_easy_init();
    if (curl) {
        CURLcode result;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteToStringCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
        result = curl_easy_perform(curl);
        if (result != CURLE_OK)
            printf("error: %s\n", curl_easy_strerror(result));
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return data;
}