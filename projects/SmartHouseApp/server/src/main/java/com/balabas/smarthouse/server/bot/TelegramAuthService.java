package com.balabas.smarthouse.server.bot;

import java.util.Set;

public interface TelegramAuthService {

    Set<String> getAllowedUserIds();
}
