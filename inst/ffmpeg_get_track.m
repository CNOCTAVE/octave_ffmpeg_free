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
## @deftypefn  {Function File} {@var{ret} =} ffmpeg_get_track (@var{input_filename})
## Get track of media file.
##
## @end deftypefn

function ret = ffmpeg_get_track (input_filename)
  if (nargin != 1)
    print_usage ();
  endif
  if (!ischar(input_filename))
    print_usage ();
  endif
  metadata = ffmpeg_get_metadata(input_filename);
  try
    ret = metadata.track;
  catch
    # fallback to empty string
    ret = "";
  end_try_catch
endfunction
