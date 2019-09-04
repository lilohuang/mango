/*
    MANGO Multimedia Development Platform
    Copyright (C) 2012-2019 Twilight Finland 3D Oy Ltd. All rights reserved.
*/
#pragma once

#include <string>
#include "../core/object.hpp"
#include "format.hpp"
#include "compression.hpp"
#include "exif.hpp"

namespace mango
{
    class Surface;

    struct ImageHeader
    {
        int     width = 0;   // width
        int     height = 0;  // height
        int     depth = 0;   // depth
        int     levels = 0;  // mipmap levels
        int     faces = 0;   // cubemap faces
        bool    palette = false; // palette is available
        Format  format; // preferred format (fastest available decoding)
        TextureCompression compression = TextureCompression::NONE;
    };

    class ImageDecoderInterface : protected NonCopyable
    {
    public:
        ImageDecoderInterface() = default;
        virtual ~ImageDecoderInterface() = default;

        virtual ImageHeader header() = 0;
        virtual void decode(Surface& dest, Palette* palette, int level, int depth, int face) = 0;

        // optional
        virtual Memory memory(int level, int depth, int face); // get compressed data
        virtual Exif exif(); // get exif data
    };

    struct ImageDecodeOptions
    {
        Palette* palette = nullptr; // request indexed decoding; write palette here
    };

#if 0
    struct ImageDecodeStatus
    {
        std::string name;
        bool direct;
        int frame;
    };
#endif

    class ImageDecoder : protected NonCopyable
    {
    public:
        ImageDecoder(Memory memory, const std::string& extension);
        ~ImageDecoder();

        bool isDecoder() const;
        ImageHeader header();
        void decode(Surface& dest, const ImageDecodeOptions& options = ImageDecodeOptions(), int level = 0, int depth = 0, int face = 0);

        Memory memory(int level, int depth, int face);
        Exif exif();

        typedef ImageDecoderInterface* (*CreateDecoderFunc)(Memory memory);

    protected:
        std::unique_ptr<ImageDecoderInterface> m_interface;
    };

    void registerImageDecoder(ImageDecoder::CreateDecoderFunc func, const std::string& extension);
    bool isImageDecoder(const std::string& extension);

} // namespace mango
