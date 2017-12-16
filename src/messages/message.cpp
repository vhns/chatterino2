#include "messages/message.hpp"
#include "channel.hpp"
#include "colorscheme.hpp"
#include "emojis.hpp"
#include "emotemanager.hpp"
#include "fontmanager.hpp"
#include "ircmanager.hpp"
#include "messages/link.hpp"
#include "resources.hpp"

#include <ctime>
#include <list>
#include <tuple>

namespace chatterino {
namespace messages {

bool Message::getCanHighlightTab() const
{
    return this->highlightTab;
}

void Message::setHighlight(bool value)
{
    this->highlightTab = value;
}

const QString &Message::getTimeoutUser() const
{
    return this->timeoutUser;
}

int Message::getTimeoutCount() const
{
    return this->timeoutCount;
}

const QString &Message::getUserName() const
{
    return this->userName;
}

const QString &Message::getDisplayName() const
{
    return this->displayName;
}

const QString &Message::getContent() const
{
    return this->content;
}

const std::chrono::time_point<std::chrono::system_clock> &Message::getParseTime() const
{
    return this->parseTime;
}

std::vector<Word> &Message::getWords()
{
    return this->words;
}

bool Message::isDisabled() const
{
    return this->disabled;
}

const QString &Message::getId() const
{
    return this->id;
}

namespace {

void AddCurrentTimestamp(Message *message)
{
    std::time_t t;
    time(&t);
    char timeStampBuffer[69];

    // Add word for timestamp with no seconds
    strftime(timeStampBuffer, 69, "%H:%M", localtime(&t));
    QString timestampNoSeconds(timeStampBuffer);
    message->getWords().push_back(Word(timestampNoSeconds, Word::TimestampNoSeconds,
                                       MessageColor(MessageColor::System), QString(), QString()));

    // Add word for timestamp with seconds
    strftime(timeStampBuffer, 69, "%H:%M:%S", localtime(&t));
    QString timestampWithSeconds(timeStampBuffer);
    message->getWords().push_back(Word(timestampWithSeconds, Word::TimestampWithSeconds,
                                       MessageColor(MessageColor::System), QString(), QString()));
}

}  // namespace

/// Static
Message *Message::createSystemMessage(const QString &text)
{
    Message *message = new Message;

    AddCurrentTimestamp(message);

    Word word(text, Word::Type::Default, MessageColor(MessageColor::Type::System), text, text);

    message->getWords().push_back(word);

    return message;
}

Message *Message::createTimeoutMessage(const QString &username, const QString &durationInSeconds,
                                       const QString &reason)
{
    Message *message = new Message;

    AddCurrentTimestamp(message);

    QString text;

    text.append(username);
    text.append(" has been timed out");

    // TODO: Implement who timed the user out

    text.append(" for ");
    text.append(durationInSeconds);
    bool ok = true;
    int timeoutDuration = durationInSeconds.toInt(&ok);
    text.append(" second");
    if (ok && timeoutDuration > 1) {
        text.append("s");
    }

    if (reason.length() > 0) {
        text.append(": \"");
        text.append(reason);
        text.append("\"");
    }
    text.append(".");

    Word word(text, Word::Type::Default, MessageColor(MessageColor::Type::System), text, text);

    message->getWords().push_back(word);

    return message;
}

}  // namespace messages
}  // namespace chatterino
