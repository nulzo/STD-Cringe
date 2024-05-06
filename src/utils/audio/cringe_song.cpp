/*
 * MIT License
 *
 * Copyright (c) 2023 @nulzo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "utils/audio/cringe_song.h"

#include <utility>

std::string CringeSong::get_title() { return title; }

std::string CringeSong::get_artist() { return artist; }

std::string CringeSong::get_thumbnail() { return thumbnail; }

std::string CringeSong::get_formatted_duration() { return duration; }

std::string CringeSong::get_url() { return url; }

std::string CringeSong::get_filter() { return filter; }

std::string CringeSong::get_upload_date() { return upload_date; }

std::string CringeSong::get_comments() { return comment_count; }

std::string CringeSong::get_subscribers() { return subscriber_count; }

std::string CringeSong::get_view_count() { return view_count; }

dpp::slashcommand_t CringeSong::get_event() { return event; }

CringeSong::CringeSong(std::string title, std::string artist,
                       std::string thumbnail, std::string duration,
                       std::string view_count, std::string comment_count,
                       std::string upload_date, std::string subscriber_count,
                       std::string filter, std::string url,
                       dpp::slashcommand_t &event)
    : title(std::move(title)), artist(std::move(artist)),
      thumbnail(std::move(thumbnail)), duration(std::move(duration)),
      filter(std::move(filter)), url(std::move(url)),
      upload_date(std::move(upload_date)),
      comment_count(std::move(comment_count)),
      subscriber_count(std::move(subscriber_count)),
      view_count(std::move(view_count)), event(std::move(event)) {}
