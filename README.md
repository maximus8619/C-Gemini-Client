# Google Gemini API Interaction

This project demonstrates how to interact with the Google Gemini API using C++. The provided code enables users to input a question through the console, which is then sent to the Gemini API. The response from the API is displayed in the console. This guide will help you understand how to set up, use, and secure this project.

## Prerequisites

- **C++ Compiler**: A modern C++ compiler that supports C++11 or later.
- **libcurl**: A library for transferring data with URLs.
- **nlohmann/json**: A JSON library for C++ for easy handling of JSON.

### Dependencies

- **cURL**: This project uses libcurl for making HTTP requests to the Gemini API. Make sure `libcurl` is installed on your system.
- **nlohmann/json**: The `nlohmann/json` library is used for parsing JSON responses from the API. You can add it to your project by downloading it from [GitHub](https://github.com/nlohmann/json).

## Installation

1. **Clone the Repository** (if applicable):
   ```sh
   git clone <repository-url>
   ```

2. **Install Dependencies**:
   - Install libcurl:
     - **Ubuntu**: `sudo apt-get install libcurl4-openssl-dev`
     - **Windows**: Use vcpkg or download a compatible version from [cURL's website](https://curl.se/download.html).
   - Include `nlohmann/json.hpp` in your project. You can add it via package managers like vcpkg or just download the header file.

## Setting up the API Key

The code expects an API key for the Google Gemini API. You should set this key in your environment variables:

- **Linux/macOS**:
  ```sh
  export GEMINI_API_KEY=your_api_key_here
  ```
- **Windows**:
  ```cmd
  set GEMINI_API_KEY=your_api_key_here
  ```

## How to Use

1. **Compile the Code**:
   Compile the code using your preferred C++ compiler. Make sure to link against `libcurl`.
   ```sh
   g++ -o gemini_interact main.cpp -lcurl
   ```

2. **Run the Application**:
   Run the compiled application.
   ```sh
   ./gemini_interact
   ```

3. **Enter Your Query**:
   The program will prompt you to enter a question. Type your question and press Enter. To exit the application, type `exit`.

## Code Overview

- **WriteCallback**: A function used by libcurl to capture the response from the HTTP request.
- **interactWithGeminiAPI**: This function takes the user input and sends it to the Google Gemini API, returning the response.
- **main**: The main loop where the user is prompted to input questions and the interaction with the API occurs.

## Error Handling

The code includes error handling for different scenarios:
- If the API key is not found in the environment variables, the program will terminate with an appropriate message.
- If the API request fails, detailed error messages are provided to help understand what went wrong.
- Warnings are included to ensure sensitive information, like the API key, is kept secure.

## Security Considerations

- **API Key Management**: Ensure that your API key is kept secure and is not logged or hardcoded directly in the code. The code fetches the API key from an environment variable to mitigate accidental exposure.
- **Sensitive Data**: Do not commit your API key to version control.

## Troubleshooting

- **cURL Errors**: If you encounter errors related to cURL, ensure that `libcurl` is properly installed and linked.
- **API Key Issues**: If you receive an error regarding the API key, double-check that it is correctly set in your environment variables.
- **JSON Parsing Errors**: The application uses the `nlohmann::json` library for parsing API responses. Ensure that the response format has not changed or been misconfigured.

## Contribution

Feel free to submit issues or pull requests to enhance this project. Contributions are always welcome!

## License

This project is licensed under the MIT License. See the `LICENSE` file for more details.

## Disclaimer

The Google Gemini API may have specific usage limits or terms of service. Ensure you comply with those terms when using the API.

