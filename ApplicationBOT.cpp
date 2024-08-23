// Creater -> KosmoNavt.cpp | @KosmoNavt001 (Telegram)

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>

// Define bot configuration
const std::string BOT_TOKEN = "YOURBOTTOKEN"; // Replace with bot token
const std::string CHANNEL_ID = "YOURCHANNELID"; // ID Channel 
const int ADMIN_USER_ID = 779; // User ID

// Function to save user ID to a file
void saveUserId(int userId) {
    std::ofstream file("Test.txt", std::ios::app);
    if (!file) {
        std::cerr << "Error opening file: " << std::strerror(errno) << std::endl;
        return;
    }
    file << userId << ";";
    file.close();
    std::cout << "ID " << userId << " successfully saved to Test.txt" << std::endl;
}

// Function to read user IDs from a file
std::vector<int> readUserIdsFromFile() {
    std::ifstream file("Test.txt");
    if (!file) {
        std::cerr << "Error opening file: " << std::strerror(errno) << std::endl;
        return {};
    }

    std::vector<int> userIds;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        while (std::getline(iss, token, ';')) {
            if (!token.empty()) {
                userIds.push_back(std::stoi(token));
            }
        }
    }
    file.close();
    return userIds;
}

int main() {
    // Initialize bot
    Telegraf bot(BOT_TOKEN);

    // Set up new chat members event handler
    bot.onNewChatMembers([](const Message& message) {
        const std::vector<Member>& newMembers = message.newChatMembers;

        for (const auto& member : newMembers) {
            int userId = member.id;

            // Send greeting message to the user
            bot.sendMessage(userId, "Привет! Спасибо за участие в нашем канале!");
            saveUserId(userId);

            // Add the user to the channel
            bot.addChatMember(CHANNEL_ID, userId);
        }
    });

    // Set up /NewPost command handler
    bot.command("/NewPost", [](const Message& message) {
        if (message.from.id == ADMIN_USER_ID) {
            std::string postMessage = message.text.substr(8);

            if (!postMessage.empty()) {
                std::vector<int> userIds = readUserIdsFromFile();

                for (int userId : userIds) {
                    try {
                        bot.sendMessage(userId, postMessage);
                    } catch (const std::exception& e) {
                        std::cerr << "Failed to send message to user " << userId << ": " << e.what() << std::endl;
                    }
                }
                message.reply("Сообщение успешно отправлено всем пользователям.");
            } else {
                message.reply("Пожалуйста, укажите текст сообщения после команды /NewPost.");
            }
        } else {
            message.reply("У вас нет доступа к этой команде.");
        }
    });

    // Start the bot
    bot.start().then([&bot]() {
        std::cout << "Бот запущен и готов к работе!" << std::endl;
    }).catch([](const std::exception& e) {
        std::cerr << "Ошибка при запуске бота: " << e.what() << std::endl;
    });

    return 0;
}
