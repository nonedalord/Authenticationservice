cmake_minimum_required(VERSION 3.8)

project(AuthenticationService)

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH /usr/local/include)

# find_package(spdlog REQUIRED)
find_package(Boost 1.79 REQUIRED COMPONENTS json)
#find_library(PQXX_LIB pqxx)
#find_library(PQ_LIB pq)

set(OPENSSL_ROOT_DIR /opt/openssl-3.0)
set(OPENSSL_INCLUDE_DIR /opt/openssl-3.0/include)
set(OPENSSL_LIBRARIES /opt/openssl-3.0/lib64)

find_package(OpenSSL REQUIRED)

include_directories(${Boost_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})

file(GLOB authentication_src "authentication/*.cpp")
file(GLOB authentication_web_server_src "authentication/web_server/*.cpp")
file(GLOB authentication_crypto_src "authentication/crypto_utils/*.cpp" "authentication/crypto_utils/jwt_token/*.cpp" "authentication/crypto_utils/password_argon2id/*.cpp")

add_executable(AuthenticationService 
    ${authentication_src} 
    ${authentication_web_server_src} 
    ${authentication_crypto_src}
)

target_link_libraries(AuthenticationService 
    PRIVATE 
    pthread 
  # ${PQXX_LIB} 
  # ${PQ_LIB} 
    Boost::boost 
    Boost::json 
    OpenSSL::SSL 
    OpenSSL::Crypto 
  # spdlog::spdlog_header_only
)
