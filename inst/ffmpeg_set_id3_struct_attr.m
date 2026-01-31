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
## @deftypefn  {Function File} {@var{ret} =} ffmpeg_set_id3_struct_attr 
## (@var{id3_struct}, @var{attr}, @var{value})
## Set any attribute of ID3 struct.
## attr value should be a string.
## value value should be a string.
##
## @end deftypefn
  
function id3_struct = ffmpeg_set_id3_struct_attr (id3_struct, attr, value)
  if (nargin != 3)
    print_usage ();
  endif
  if (!isstruct(id3_struct))
    print_usage ();
  endif
  if (!ischar(attr))
    print_usage ();
  endif
  if (!ischar(value))
    print_usage ();
  endif
  id3_struct.(attr) = value;
endfunction