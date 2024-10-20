#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <cstdlib> // For getenv

using json = nlohmann::json;

/**
 * @brief Callback function to capture the response from curl.
 *
 * This function is called by libcurl as soon as data is received that needs to be saved.
 * The function appends the received data to the user-provided string.
 *
 * @param contents Pointer to the received data.
 * @param size Size of a single data element.
 * @param nmemb Number of data elements.
 * @param userp Pointer to the user-provided string where the data will be appended.
 * @return The number of bytes actually processed.
 */
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

/**
 * @brief Interacts with the Google Gemini API to process user input and generate a response.
 *
 * This function takes an API key and user input as a string, sends the input to the Google Gemini API,
 * and returns the response generated by the API.
 *
 * @param api_key The API key for authentication with the Google Gemini API.
 * @param user_input The user input to be processed by the Gemini model.
 * @return The response from the Google Gemini API as a string. If an error occurs, an error message is returned.
 */
std::string interactWithGeminiAPI(const std::string& api_key, const std::string& user_input) {
    std::string api_url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-pro:generateContent?key=" + api_key;
    std::string request_body = R"({"contents": [{"parts": [{"text": ")" + user_input + R"("}]}]})";

    // String to store the response
    std::string response_string;
    CURL* curl;
    CURLcode res;

    // Initialize CURL globally
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        // Set the URL and options
        curl_easy_setopt(curl, CURLOPT_URL, api_url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request_body.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

        // Perform the request
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            try {
                // Parse the response
                auto response_json = json::parse(response_string);
                if (response_json.contains("error")) {
                    std::cerr << "API Error: " << response_json["error"]["message"].get<std::string>() << " (Code: " << response_json["error"]["code"].get<int>() << ")" << std::endl;
                } else if (response_json.contains("candidates")) {
                    return response_json["candidates"][0]["content"]["parts"][0]["text"].get<std::string>();
                } else {
                    std::cerr << "Unexpected response format. Full response: " << response_string << std::endl;
                }
            } catch (json::parse_error& e) {
                std::cerr << "JSON parse error: " << e.what() << ". Response content: " << response_string << std::endl;
            } catch (std::exception& e) {
                std::cerr << "General error: " << e.what() << std::endl;
            }
        }

        // Cleanup
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    } else {
        std::cerr << "Failed to initialize CURL." << std::endl;
    }

    // Cleanup CURL globally
    curl_global_cleanup();
    return "Error processing the request.";
}

int main() {
    // Retrieve the API key from environment variables
const char* api_key_env = getenv("GEMINI_API_KEY");
if (api_key_env == nullptr) {
    std::cerr << "API key not found in environment variables." << std::endl;
    return 1;
}
std::string api_key = api_key_env;


    // Warning: Ensure that the API key is kept secure and not exposed in logs or error messages
    if (api_key.empty()) {
        std::cerr << "API key is empty. Please set a valid API key." << std::endl;
        return 1;
    }

    while (true) {
        // Get user input
        std::string user_input;
        std::cout << "Enter your question (or 'exit' to quit): ";
        std::getline(std::cin, user_input);

        if (user_input == "exit") {
            break;
        }

        // Get response from the API
        std::string answer = interactWithGeminiAPI(api_key, user_input);
        if (!answer.empty() && answer != "Error processing the request.") {
            std::cout << "Response from GEMINI: " << answer << std::endl;
        } else {
            std::cerr << "No valid response received from the API." << std::endl;
        }
    }

    return 0;
}
