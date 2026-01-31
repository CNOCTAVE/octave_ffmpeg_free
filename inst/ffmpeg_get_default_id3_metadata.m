## Copyright (C) 2024-2026 Yu Hongbo <yuhongbo@member.fsf.org>, 
##                    CNOCTAVE <cnoctave@qq.com>
##
## This program is free software; you can redistribute it and/or modify it under
## the terms of the GNU General Public License as published by the Free Software
## Foundation; either version 3 of the License, or (at your option) any later
## version.
##
## This program is distributed in the hope that it will be useful, but WITHOUT
## ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
## FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
## details.
##
## You should have received a copy of the GNU General Public License along with
## this program; if not, see <http://www.gnu.org/licenses/>.

## -*- texinfo -*-
## @deftypefn  {Function File} {@var{ret} =} ffmpeg_get_default_id3_metadata ()
## Get default ID3 metadata for creating or modifing media file.
##
## @end deftypefn

function ret = ffmpeg_get_default_id3_metadata ()
  ret = struct ();
  ret.album_artist = "Unknown Album Artist";
  ret.album = "Unknown Album";
  ret.artist = "Unknown Artist";
  ret.comment = "Unknown Comment";
  ret.composer = "Unknown Composer";
  ret.date = "Unknown Date";
  ret.disc = "0";
  ret.genre = "Unknown Genre";
  ret.language = "Unknown Language";
  ret.mood = "Unknown Mood";
  ret.("set") = "Unknown Set";
  ret.subtitle = "Unknown Subtitle";
  ret.title = "Unknown Title";
  ret.track = "0";
endfunction
