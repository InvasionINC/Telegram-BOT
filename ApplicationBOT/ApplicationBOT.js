// Created by -> KosmoNavt.cpp ; @KosmoNavt001 (Telegram)

const Telegraf = require('telegraf');
const fs = require('fs');

const bot = new Telegraf('6990691963:AAEwcBHxTjyitgJyAE3AMxyS54ALAxmjamA'); // Bot token
const channelId = '-1002235172680'; // Your channel ID
const adminUserId = 5952558257; // Your ID
console.log('Бот запущен и готов к работе!');

function saveUserId(userId) {
    fs.appendFile('Test.txt', `${userId};`, (err) => {
      if (err) {
        console.error(`Ошибка при записи в файл: ${err.message}`);
      } else {
        console.log(`ID ${userId} успешно сохранен в Test.txt`);
      }
    });
  }
  
  function readUserIdsFromFile() {
    return new Promise((resolve, reject) => {
      fs.readFile('Test.txt', 'utf8', (err, data) => {
        if (err) {
          return reject(err);
        }
        const userIds = data.split(';').filter(id => id).map(Number);
        resolve(userIds);
      });
    });
  }
  
  bot.on('newchatmembers', async (ctx) => {
    const newMembers = ctx.message.newchatmembers;
  
    for (let member of newMembers) {
      const userId = member.id;
  
      await bot.telegram.sendMessage(userId, 'Привет! Спасибо за участие в нашем канале!');
      saveUserId(userId);
      await ctx.telegram.addChatMember(channelId, userId);
    }
  });
  
  bot.command('/NewPost', async (ctx) => {
    if (ctx.message.from.id === adminUserId) {
      const postMessage = ctx.message.text.split('/NewPost ')[1];
  
      if (postMessage) {
        const userIds = await readUserIdsFromFile();
  
        for (const userId of userIds) {
          try {
            await bot.telegram.sendMessage(userId, postMessage);
          } catch (error) {
            console.error(`Не удалось отправить сообщение пользователю ${userId}: ${error.message}`);
          }
        }
        ctx.reply('Сообщение успешно отправлено всем пользователям.');
      } else {
        ctx.reply('Пожалуйста, укажите текст сообщения после команды /NewPost.');
      }
    } else {
      ctx.reply('У вас нет доступа к этой команде.');
    }
  });
  
  bot.launch().then(() => {
    console.log('Бот запущен и готов к работе!');
  }).catch((error) => {
    console.error(`Ошибка при запуске бота: ${error.message}`);
  });

// This bot sends a greeting in private messages to the user
// If he threw a request into the channel
// The bot accepts his request. 
// Team /NewPost creates a post that will be sent to all bot users
// Bot users are saved to a file Test.txt
