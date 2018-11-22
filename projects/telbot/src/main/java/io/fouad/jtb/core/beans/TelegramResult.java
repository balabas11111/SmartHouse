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
 * This class represents the response received from Telegram
 * upon sending requests through the API.
 */
@Data
@ToString
@EqualsAndHashCode
public class TelegramResult<R>
{
	/**
	 * indicates whether the request was successful or not.
	 */
	@JsonProperty("ok")
	private boolean ok;
	
	/**
	 * optional. the error code when ok == false.
	 */
	@JsonProperty("error_code")
	private Integer errorCode;
	
	/**
	 * result of the query.
	 */
	@JsonProperty("result")
	private R result;
	
	/**
	 * optional. a human-readable description of the result.
	 */
	@JsonProperty("description")
	private String description;
	
	@JsonProperty("left_chat_participant")
	private String left_chat_participant;
	
	public TelegramResult(){}
	
	public TelegramResult(boolean ok, Integer errorCode, R result, String description)
	{
		this.ok = ok;
		this.errorCode = errorCode;
		this.result = result;
		this.description = description;
	}
	
}