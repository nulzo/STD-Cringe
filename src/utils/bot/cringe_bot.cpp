#include "utils/bot/cringe_bot.h"
#include "utils/listeners/cringe_slashcommand.h"
#include "utils/audio/cringe_audio.h"

CringeBot::CringeBot(const std::string &token, CringeOllama ollama)
    : cluster(token, dpp::i_default_intents | dpp::i_message_content), ollama(ollama) {
	CringeAudioStreamer cringe_audio;
    cluster.on_log([this](const dpp::log_t &event) {
        cringe_logger.log_event(event);
    });
    cluster.on_slashcommand([this](const dpp::slashcommand_t &event) {
        process_slashcommand(event, *this);
    });
	cluster.on_message_delete([this](const dpp::message_delete_t &event) {

	});
	cluster.on_message_create([&](const dpp::message_create_t &event){});
    cluster.on_ready([this](const dpp::ready_t &event) {
        if (dpp::run_once<struct register_BOT_commands>()) {
            register_slashcommands(*this);
        }
        cluster.set_presence(dpp::presence(dpp::ps_online, dpp::at_custom, "currently in development"));
    });
	cringe_logger.log_start();
    cluster.start(dpp::st_wait);
}

CringeBot::~CringeBot() = default;
