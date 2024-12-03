// Copyright (C) 2024 Yu Hongbo <yuhongbo@member.fsf.org>, 
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


DEFUN_DLD (ffmpeg_get_metadata, args, , "\
-*- texinfo -*-\n\
@deftypefn {Loadable Function} @var{metadata} = ffmpeg_get_metadata(@var{input_filename})\n\
Input mp3 filename @var{input_filename}, return @var{metadata}.\n\
@end deftypefn\n\
")
{
  octave_value retval;
  if (args.length () != 1)
    print_usage ();
  if (! args(0).is_string ())
    error ("ffmpeg_get_metadata: input_filename should be a string");
  if (args(0).isempty ())
    error ("ffmpeg_get_metadata: input_filename should not be empty");
  const char* input_filename = args(0).string_value ().c_str ();

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

  // 创建一个 octave_map 来存储元数据
  octave_map metadata;

  // 遍历 AVFormatContext 的元数据
  AVDictionaryEntry* tag = nullptr;
  while ((tag = av_dict_get(formatContext->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))) {
    std::string entry = std::string(tag->key) + ": " + std::string(tag->value);
    metadata.setfield(std::string(tag->key), Cell(octave_value(std::string(tag->value))));
  }

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
