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
## @deftypefn  {Function File} {@var{ret} =} ffmpeg_set_id3_struct_mood 
## (@var{id3_struct}, @var{value})
## Set mood of ID3 struct.
## mood value should be a string.
##
## @end deftypefn
  
function id3_struct = ffmpeg_set_id3_struct_mood (id3_struct, value)
  if (nargin != 2)
    print_usage ();
  endif
  if (!isstruct(id3_struct))
    print_usage ();
  endif
  if (!ischar(value))
    print_usage ();
  endif
  id3_struct.mood = value;
endfunction