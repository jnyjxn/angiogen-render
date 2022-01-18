FROM python:3
RUN apt update -y && apt-get install -y \
    build-essential \
    wget \
    git \
    curl \
    gcc \
    g++ \
    cmake \
    subversion \
  && rm -rf /var/lib/apt/lists/*

WORKDIR /external

# Install Miniconda
RUN curl -so /external/miniconda.sh https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh \
 && chmod +x /external/miniconda.sh \
 && /external/miniconda.sh -b -p /external/miniconda \
 && rm /external/miniconda.sh

ENV PATH=/external/miniconda/bin:$PATH

# Create a Python 3.8 environment
RUN conda install -y conda-build \
 && conda create -y --name py38 python=3.8 \
 && conda clean -ya

ENV CONDA_DEFAULT_ENV=py38
ENV CONDA_PREFIX=/external/miniconda/envs/$CONDA_DEFAULT_ENV
ENV PATH=$CONDA_PREFIX/bin:$PATH
ENV CONDA_AUTO_UPDATE_CONDA=false

# add headless support for pyrender
# https://pyrender.readthedocs.io/en/latest/install/index.html
# install OSMesa
RUN apt update -y \
  && wget https://github.com/mmatl/travis_debs/raw/master/xenial/mesa_18.3.3-0.deb \
  && dpkg -i ./mesa_18.3.3-0.deb || true \
  && apt install -f -y
# install a Compatible Fork of PyOpenGL
RUN git clone https://github.com/mmatl/pyopengl.git \
  && pip install ./pyopengl

RUN echo "export PATH=$CONDA_PREFIX/bin:$PATH" >> /etc/profile
RUN echo "export PYTHONPATH=/miniconda/envs/lib/python3.8/site-packages:$PYTHONPATH" >> /etc/profile
RUN echo "export NCCL_LL_THRESHOLD=0" >> /etc/profile
RUN echo "umask 002" >> /etc/profile

# set the working directory in the container
WORKDIR /external

# copy the dependencies file to the working directory
COPY requirements.txt ./

 # install dependencies
RUN pip install -r requirements.txt


# RUN apt-get update && apt-get install -y cmake build-essential
# RUN git clone https://github.com/tomopy/tomopy.git
# WORKDIR /external/tomopy
# RUN python setup.py install

WORKDIR /external

# Get gvxr
RUN svn checkout -r 2109 svn://zedbluffer@svn.code.sf.net/p/gvirtualxray/code/branches/better_support_of_GL_context gvirtualxray

RUN mkdir gvirtualxray/bin

# Change workdir
WORKDIR /external/gvirtualxray/bin

# Build
RUN apt-get update && apt-get install -y libxi-dev swig libxmu-dev mesa-utils
RUN apt-get update && apt-get install -y libglew-dev glew-utils 
RUN apt-get update && apt-get install -y libxrandr-dev libxcursor-dev
RUN apt-get update && apt-get install -y libxinerama-dev libxxf86vm-dev
RUN apt-get update && apt-get install -y libglfw3 libglfw3-dev
ENV CC=/usr/bin/gcc
ENV CXX=/usr/bin/g++
RUN cmake \
        -DCMAKE_C_COMPILER:STRING=/usr/bin/gcc \
        -DCMAKE_CXX_COMPILER:STRING=/usr/bin/g++ \
        -DCMAKE_BUILD_TYPE:STRING=Release \
        -DBUILD_SIMPLEGVXR:BOOL=ON \
        -DBUILD_TESTING:BOOL=ON \
        -DUSE_LIBTIFF:BOOL=ON \
        -DBUILD_TESTING:BOOL=ON \
        -DUSE_SYSTEM_GLFW:BOOL=OFF \
        -DUSE_SYSTEM_GLEW:BOOL=OFF \
        -DBUILD_WRAPPER_PYTHON3:BOOL=ON \
        -DGLEW_USE_STATIC_LIBS:BOOL=ON \
        -S .. \
        -B .

RUN make glew -j48
RUN mkdir gvxr/glew-install/lib64
RUN cp gvxr/glew-install/lib/lib*.a gvxr/glew-install/lib64
RUN make glfw -j48
RUN make googletest -j48

RUN mkdir third_party/lib64
RUN cp third_party/lib/lib*.a third_party/lib64
RUN make assimp -j48
RUN make gVirtualXRay -j48
RUN make SimpleGVXR -j48
RUN make gvxrPython3 -j48
RUN make -j48

# Install
RUN make install

RUN apt-get update && apt-get install -y     libglvnd0 \
    libgl1 \
    libglx0 \
    libegl1 \
    libxext6 \
    libx11-6 

# Add in Python Path
ENV PYTHONPATH=/usr/local/gvxrWrapper-1.0.6/python3/test.py
ENV EGL_PLATFORM=x11

# Env vars for the nvidia-container-runtime.
ENV NVIDIA_VISIBLE_DEVICES=all
ENV NVIDIA_DRIVER_CAPABILITIES=graphics,utility,compute

WORKDIR /angiogen

RUN pip install pydantic
RUN pip install scikit-image tqdm 
RUN pip install fastapi "uvicorn[standard]"

COPY angiogen .
RUN cp /usr/local/gvxrWrapper-1.0.6/python3/* .

CMD ["python3", "main.py"]

# CMD [ "python3", "./test-offscreen.py" ]
# CMD [ "echo", "hello" ]
