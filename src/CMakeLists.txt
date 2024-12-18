include_directories(${CMAKE_CURRENT_BINARY_DIR})

kcoreaddons_add_plugin(
    katediscordrpcplugin
    INSTALL_NAMESPACE "kf6/ktexteditor"
    SOURCES discordrpcplugin.cpp discordrpcconfigpage.cpp
)

target_link_libraries(katediscordrpcplugin
    KF6::TextEditor
    LibDiscord
)
target_compile_definitions(katediscordrpcplugin PUBLIC DISCORD_ID="${DISCORD_ID}")
