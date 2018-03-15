/**
 * filewatcher - a simple auditing utility for macOS
 * Copyright (C) 2018 meliot
 *
 * This file is part of filewatcher.
 *
 * filewatcher is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * filewatcher is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with filewatcher. If not, see <http://www.gnu.org/licenses/>.
 *
 * auditing.h
 *
 * This file manage the auditing of events. For every event, it fetch tokens
 * from the audit pipe and parse the token structs to return an auditEvent
 * struct with all informations about the single event.
 *
 */

#include "common.h"

struct auditEvent getEvent(FILE* auditFile);
