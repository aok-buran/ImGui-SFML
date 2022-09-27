include(FetchContent)



# SFML
FetchContent_Declare(
        sfml
        URL https://github.com/SFML/SFML/archive/refs/tags/2.5.1.zip
        URL_MD5 2c4438b3e5b2d81a6e626ecf72bf75be
)

add_subdirectory(sfml)