/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Fouad Almalki
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

package io.fouad.jtb.core.beans;

import com.fasterxml.jackson.annotation.JsonProperty;

import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.ToString;


/**
 * This class represents a Telegram message.
 */
@Data
@ToString
@EqualsAndHashCode
public class Message
{
	/**
	 * Unique message identifier.
	 */
	@JsonProperty("id")
	private int id;
	
	@JsonProperty("message_id")
	private int messageId;
	
	/**
	 * Optional. Sender, can be empty for messages sent to channels.
	 */
	@JsonProperty("from")
	private User from;
	
	/**
	 * Date the message was sent in Unix time.
	 */
	@JsonProperty("date")
	private int date;
	
	/**
	 * Conversation the message belongs to.
	 */
	@JsonProperty("chat")
	private Chat chat;
	
	/**
	 * Optional. For forwarded messages, sender of the original message.
	 */
	@JsonProperty("forward_from")
	private User forwardFrom;
	
	/**
	 * Optional. For messages forwarded from a channel, information about
	 * the original channel.
	 */
	@JsonProperty("forward_from_chat")
	private Chat forwardFromChat;
	
	/**
	 * Optional. For forwarded messages, date the original message was sent
	 * in Unix time.
	 */
	@JsonProperty("forward_date")
	private Integer forwardDate;
	
	/**
	 * Optional. For replies, the original message. Note that the Message
	 * object in this field will not contain further reply_to_message fields
	 * even if it itself is a reply.
	 */
	@JsonProperty("reply_to_message")
	private Message replyToMessage;
	
	/**
	 * Optional. Date the message was last edited in Unix time.
	 */
	@JsonProperty("edit_date")
	private int editDate;
	
	/**
	 * Optional. For text messages, the actual UTF-8 text of the message,
	 * 0-4096 characters.
	 */
	@JsonProperty("text")
	private String text;
	
	/**
	 * Optional. For text messages, special entities like usernames, URLs,
	 * bot commands, etc. that appear in the text.
	 */
	@JsonProperty("entities")
	private MessageEntity[] entities;
	
	/**
	 * Optional. Message is an audio file, information about the file.
	 */
	@JsonProperty("audio")
	private Audio audio;
	
	/**
	 * Optional. Message is a general file, information about the file.
	 */
	@JsonProperty("document")
	private Document document;
	
	/**
	 * Optional. Message is a photo, available sizes of the photo.
	 */
	@JsonProperty("photo")
	private PhotoSize[] photo;
	
	/**
	 * Optional. Message is a sticker, information about the sticker.
	 */
	@JsonProperty("sticker")
	private Sticker sticker;
	
	/**
	 * Optional. Message is a video, information about the video.
	 */
	@JsonProperty("video")
	private Video video;
	
	/**
	 * Optional. Message is a voice message, information about the file.
	 */
	@JsonProperty("voice")
	private Voice voice;
	
	/**
	 * Optional. Caption for the photo or video, 0-200 characters.
	 */
	@JsonProperty("caption")
	private String caption;
	
	/**
	 * Optional. Message is a shared contact, information about the contact.
	 */
	@JsonProperty("contact")
	private Contact contact;
	
	/**
	 * Optional. Message is a shared location, information about the location.
	 */
	@JsonProperty("location")
	private Location location;
	
	/**
	 * Optional. Message is a venue, information about the venue.
	 */
	@JsonProperty("venue")
	private Venue venue;
	
	/**
	 * Optional. A new member was added to the group, information about them
	 * (this member may be the bot itself).
	 */
	@JsonProperty("new_chat_member")
	private User newChatMember;
	
	/**
	 * Optional. A member was removed from the group, information about them
	 * (this member may be the bot itself).
	 */
	@JsonProperty("left_chat_member")
	private User leftChatMember;
	
	/**
	 * Optional. A chat title was changed to this value.
	 */
	@JsonProperty("new_chat_title")
	private String newChatTitle;
	
	/**
	 * Optional. A chat photo was change to this value.
	 */
	@JsonProperty("new_chat_photo")
	private PhotoSize[] newChatPhoto;
	
	/**
	 * Optional. Service message: the chat photo was deleted.
	 */
	@JsonProperty("delete_chat_photo")
	private Boolean deleteChatPhoto;
	
	/**
	 * Optional. Service message: the group has been created.
	 */
	@JsonProperty("group_chat_created")
	private Boolean groupChatCreated;
	
	/**
	 * Optional. Service message: the supergroup has been created.
	 * This field can‘t be received in a message coming through updates,
	 * because bot can’t be a member of a supergroup when it is created.
	 * It can only be found in reply_to_message if someone replies to
	 * a very first message in a directly created supergroup.
	 */
	@JsonProperty("supergroup_chat_created")
	private Boolean superGroupChatCreated;
	
	/**
	 * Optional. Service message: the channel has been created. This field
	 * can‘t be received in a message coming through updates, because bot
	 * can’t be a member of a channel when it is created. It can only be
	 * found in reply_to_message if someone replies to a very first message
	 * in a channel.
	 */
	@JsonProperty("channel_chat_created")
	private Boolean channelChatCreated;
	
	/**
	 * Optional. The group has been migrated to a supergroup with the specified
	 * identifier. This number may be greater than 32 bits and some programming
	 * languages may have difficulty/silent defects in interpreting it. But it
	 * smaller than 52 bits, so a signed 64 bit integer or double-precision float
	 * type are safe for storing this identifier.
	 */
	@JsonProperty("migrate_to_chat_id")
	private Long migrateToChatId;
	
	/**
	 * Optional. The supergroup has been migrated from a group with the specified
	 * identifier. This number may be greater than 32 bits and some programming
	 * languages may have difficulty/silent defects in interpreting it. But it
	 * smaller than 52 bits, so a signed 64 bit integer or double-precision float
	 * type are safe for storing this identifier.
	 */
	@JsonProperty("migrate_from_chat_id")
	private Long migrateFromChatId;
	
	/**
	 * Optional. Specified message was pinned. Note that the Message object
	 * in this field will not contain further reply_to_message fields even
	 * if it is itself a reply.
	 */
	@JsonProperty("pinned_message")
	private Message pinnedMessage;
	
	@JsonProperty("left_chat_participant")
	private User left_chat_participant;
	
	
	public Message(){}
	
	public Message(int messageId, User from, int date, Chat chat, User forwardFrom, Chat forwardFromChat,
	               Integer forwardDate, Message replyToMessage, int editDate, String text, MessageEntity[] entities,
	               Audio audio, Document document, PhotoSize[] photo, Sticker sticker, Video video, Voice voice,
	               String caption, Contact contact, Location location, Venue venue, User newChatMember,
	               User leftChatMember, String newChatTitle, PhotoSize[] newChatPhoto, Boolean deleteChatPhoto,
	               Boolean groupChatCreated, Boolean superGroupChatCreated, Boolean channelChatCreated,
	               Long migrateToChatId, Long migrateFromChatId, Message pinnedMessage)
	{
		this.messageId = messageId;
		this.from = from;
		this.date = date;
		this.chat = chat;
		this.forwardFrom = forwardFrom;
		this.forwardFromChat = forwardFromChat;
		this.forwardDate = forwardDate;
		this.replyToMessage = replyToMessage;
		this.editDate = editDate;
		this.text = text;
		this.entities = entities;
		this.audio = audio;
		this.document = document;
		this.photo = photo;
		this.sticker = sticker;
		this.video = video;
		this.voice = voice;
		this.caption = caption;
		this.contact = contact;
		this.location = location;
		this.venue = venue;
		this.newChatMember = newChatMember;
		this.leftChatMember = leftChatMember;
		this.newChatTitle = newChatTitle;
		this.newChatPhoto = newChatPhoto;
		this.deleteChatPhoto = deleteChatPhoto;
		this.groupChatCreated = groupChatCreated;
		this.superGroupChatCreated = superGroupChatCreated;
		this.channelChatCreated = channelChatCreated;
		this.migrateToChatId = migrateToChatId;
		this.migrateFromChatId = migrateFromChatId;
		this.pinnedMessage = pinnedMessage;
	}
	
	
}