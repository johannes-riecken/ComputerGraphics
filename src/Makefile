#
# Makefile to build OpenGL examples for Linux
#
# Author: Alex V. Boreskoff, <steps3d@narod.ru>, <steps3d@gmail.com>
#

OPENCLPATH = /usr/local/cuda-5.0/include/
GLUTPATH   = /usr/include/GL
CFLAGS     = -x c++ -g -I/usr/include/GL -Wall -I$(GLUTPATH)/include/GL -I$(OPENCLPATH) -I.
CONLYFLAGS = -g -I/usr/include/GL -Wall -I$(GLUTPATH)/include/GL
LIBS       = -L/usr/X11R6/lib -L$(GLUTPATH)/lib -L/usr/lib64/nvidia-experimental-310 -lglut -lGL -lXt -lX11 -lm -ljpeg -lz -lpng -lGLEW -lOpenCL
CC         = g++

MATH = vec2.o vec3.o vec4.o mat2.o mat3.o mat4.o quat.o plane.o bbox.o randUtils.o eig3.o kdop.o
TEXT = Texture.o TexImage.o TgaEncoder.o TgaDecoder.o DdsDecoder.o BmpDecoder.o JpegDecoder.o PngDecoder.o Resources.o FileSystem.o ZipFileSystem.o TexFormat.o
WRAP = Data.o Program.o VertexBuffer.o Framebuffer.o stringUtils.o glUtilities.o  UbAccessor.o Sampler.o ScreenQuad.o Sync.o Query.o TransformFeedback4.o Log.o Pipeline.o GlutWindow.o  BasicMesh.o Camera.o

ALL = ashikhmin-shirley bezier blinn-phong blinn-phong-norm cartoon cartoon-ext cellular cellular-2 charcoal cook-torrance dof ds-1 ds-2 ds-3 embm env-mapping eroded example-cubemap example-instanced fur geom-1 geom-2 glass glass-2 gooch interleaved interleaved-loc lambert marble  oren-nayar per-pixel-fog phong phong-norm pn point-sprites pom reflection render-to-texture rotate-1 rotate-2 separate simple-1 simple-2 soft-particles ssao strauss textured-3d textured-mesh torus-lit torus-unlit transform-feedback water watercolor watercolor-2 wood opencl-test opencl-test-2 mat-1 mat-2 opencl-particles

all: $(ALL)

ashikhmin-shirley: ashikhmin-shirley.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

bezier: bezier.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

blinn-phong: blinn-phong.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

blinn-phong-norm: blinn-phong-norm.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

cartoon: cartoon.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

cartoon-ext: cartoon-ext.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

cellular: cellular.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

cellular-2: cellular-2.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

charcoal: charcoal.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

cook-torrance: cook-torrance.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

dof: dof.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

ds-1: ds-1.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

ds-2: ds-2.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

ds-3: ds-3.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

embm: embm.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

env-mapping: env-mapping.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

eroded: eroded.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

example-cubemap: example-cubemap.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

example-instanced: example-instanced.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

fur: fur.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

geom-1: geom-1.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

geom-2: geom-2.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

glass: glass.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

glass-2: glass-2.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

gooch: gooch.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

interleaved: interleaved.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

interleaved-loc: interleaved-loc.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

lambert: lambert.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

marble: marble.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

oren-nayar: oren-nayar.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

per-pixel-fog: per-pixel-fog.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

phong: phong.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

phong-norm: phong-norm.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

pn: pn.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

point-sprites: point-sprites.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

pom: pom.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

reflection: reflection.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

render-to-texture: render-to-texture.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

rotate-1: rotate-1.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

rotate-2: rotate-2.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

separate: separate.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

simple-1: simple-1.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

simple-2: simple-2.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

soft-particles: soft-particles.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

ssao: ssao.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

strauss: strauss.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

textured-3d: textured-3d.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

textured-mesh: textured-mesh.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

torus-lit: torus-lit.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

torus-unlit: torus-unlit.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

transform-feedback: transform-feedback.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

water: water.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

watercolor: watercolor.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

watercolor-2: watercolor-2.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

wood: wood.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

opencl-test:opencl-test.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

opencl-test-2: opencl-test-2.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

mat-1: mat-1.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

mat-2: mat-2.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

opencl-particles: opencl-particles.o $(MATH) $(TEXT) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

clean:
	rm -r -f $(ALL) *.o 2> /dev/null

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $<


%.o: eig3x3/%.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $<


