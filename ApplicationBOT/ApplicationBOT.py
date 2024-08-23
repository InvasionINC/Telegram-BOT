# Создал -> КосмоНавт.cpp | @KosmoNavt001 (Telegram)

from aiogram import Bot, Dispatcher, executor, types


API_TOKEN = 'Токен бота через Бот фазер'
CHANNEL_ID = 'Айди канала'
CAPTCHA = 'Я человек'

bot = Bot(token=API_TOKEN)
dp = Dispatcher(bot)

def check_captcha(captcha_response):
    return captcha_response == CAPTCHA

@dp.message_handler(content_types=types.ContentTypes.NEW_CHAT_MEMBERS)
async def on_new_chat_members(message: types.Message):
    if str(message.chat.id) == CHANNEL_ID:
        for member in message.new_chat_members:
            await bot.send_message(member.id, 'Напишите: "Я человек", чтобы пройти капчу')
            user_response = message.text
            if check_captcha(user_response):
                await bot.promote_chat_member(chat_id=CHANNEL_ID, user_id=member.id, can_send_messages=True)
                await bot.send_message(member.id, 'Капча пройдена, добро пожаловать!')
            else:
                await bot.send_message(member.id, 'Капча не пройдена, попробуйте снова')

if name == 'main':
    executor.start_polling(dp, skip_updates=True)
