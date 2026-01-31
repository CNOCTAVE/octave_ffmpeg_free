// Copyright (C) 2024-2026 Yu Hongbo <yuhongbo@member.fsf.org>, 
//                    CNOCTAVE <cnoctave@qq.com>
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 3 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include <vector>
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libswresample/swresample.h>
}

#include <octave/oct.h>
#include <octave/ov-base.h>


DEFUN_DLD (ffmpeg_set_metadata, args, , "\
-*- texinfo -*-\n\
@deftypefn {Loadable Function} @var{metadata} = ffmpeg_set_metadata(@var{input_filename}, @var{metadata_dict})\n\
Input mp3 filename @var{input_filename} and metadata dictionary (ID3 dict) @var{metadata_dict},\n\
set metadata for the file.\n\
@end deftypefn\n\
")
{
  octave_value retval;
  if (args.length () != 2)
    print_usage ();
  if (! args(0).is_string ())
    error ("ffmpeg_set_metadata: input_filename should be a string");
  if (args(0).isempty ())
    error ("ffmpeg_set_metadata: input_filename should not be empty");
  if (! args(1).isstruct ())
    error ("ffmpeg_set_metadata: metadata_dict should be a map");
  const char* input_filename = args(0).string_value ().c_str ();

  octave_map metadata = args(1).map_value ();

  AVFormatContext* formatContext = nullptr;
  // Open input file, and allocate format context for the input file
  if (avformat_open_input(&formatContext, input_filename, nullptr, nullptr) != 0) {
    std::string concatenated = std::string("Could not open input file: ") + std::string(input_filename);
    error ("%s", concatenated.c_str());
  }

  // Retrieve stream information
  if (avformat_find_stream_info(formatContext, nullptr) < 0) {
    avformat_close_input(&formatContext);
    error ("Could not find stream information");
  }

  // 遍历 AVFormatContext 的元数据
  string_vector keys = metadata.keys();

  try{
    for (int i = 0; i < keys.numel (); ++i)
    {
      std::string key = keys(i);
      octave_idx_type r = 0, c = 0;
      Cell C = metadata.getfield(key);
      const octave_value &v = C.elem(r, c);
      std::string val = v.string_value();
      av_dict_set(&(formatContext->metadata), key.c_str(), val.c_str(), 0);
    }
  } catch (const std::exception& e) {
    warning ("some key/value pairs may not be set");
    warning ("%s", e.what());
  }
  av_dict_free(&(formatContext->metadata));

  // Find the first audio stream
  AVCodecParameters* codecParameters = nullptr;
  int audioStreamIndex = -1;
  for (unsigned int i = 0; i < formatContext->nb_streams; i++) {
    if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
      audioStreamIndex = i;
      codecParameters = formatContext->streams[i]->codecpar;
      break;
    }
  }

  if (audioStreamIndex == -1) {
    avformat_close_input(&formatContext);
    error ("Could not find an audio stream in the input file");
  }

  // Find the decoder for the audio stream
  const AVCodec* codec = avcodec_find_decoder(codecParameters->codec_id);
  if (codec == nullptr) {
    avformat_close_input(&formatContext);
    error ("Unsupported codec");
  }

  AVCodecContext* codecContext = avcodec_alloc_context3(codec);
  if (avcodec_parameters_to_context(codecContext, codecParameters) < 0) {
    avcodec_free_context(&codecContext);
    avformat_close_input(&formatContext);
    error ("Could not copy codec parameters to codec context");
  }

  // Open codec
  if (avcodec_open2(codecContext, codec, nullptr) < 0) {
    avcodec_free_context(&codecContext);
    avformat_close_input(&formatContext);
    error ("Could not open codec");
  }

  // Clean up and close
  avcodec_free_context(&codecContext);
  avformat_close_input(&formatContext);
  retval = octave_value (metadata);

  return retval;
}
