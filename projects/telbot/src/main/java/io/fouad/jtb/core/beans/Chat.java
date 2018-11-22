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
import io.fouad.jtb.core.enums.ChatType;
import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.ToString;

/**
 * This object represents a chat.
 */
@Data
@ToString
@EqualsAndHashCode
public class Chat
{
	/**
	 * Unique identifier for this chat. This number may be greater than 32 bits
	 * and some programming languages may have difficulty/silent defects in
	 * interpreting it. But it smaller than 52 bits, so a signed 64 bit integer
	 * or double-precision float type are safe for storing this identifier.
	 */
	@JsonProperty("id")
	private long id;
	
	/**
	 * Type of chat, can be either “private”, “group”, “supergroup” or “channel”.
	 */
	@JsonProperty("type")
	private ChatType type;
	
	/**
	 * Optional. Title, for channels and group chats.
	 */
	@JsonProperty("title")
	private String title;
	
	/**
	 * Optional. Username, for private chats, supergroups and channels if available.
	 */
	@JsonProperty("username")
	private String username;
	
	/**
	 * Optional. First name of the other party in a private chat.
	 */
	@JsonProperty("first_name")
	private String firstName;
	
	/**
	 * Optional. Last name of the other party in a private chat.
	 */
	@JsonProperty("last_name")
	private String lastName;
	
	@JsonProperty("all_members_are_administrators")
	private String all_members_are_administrators;
	
	public Chat(){}
	
	public Chat(long id, ChatType type, String title, String username, String firstName, String lastName)
	{
		this.id = id;
		this.type = type;
		this.title = title;
		this.username = username;
		this.firstName = firstName;
		this.lastName = lastName;
	}
	
}