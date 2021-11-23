FROM nvidia/cuda:11.4.2-base-ubuntu20.04

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y \
    cmake \
    wget \
    curl \
    git \
    rsync \
    sudo \
    zip \ 
    vim \
    ssh \
    unzip \
    screen \
    openssh-server \
    pkg-config \
    unrar \
    build-essential \
    freeglut3-dev \
    libatlas-base-dev \
    libboost-all-dev \
    libcgal-dev \ 
    libeigen3-dev \
    libfreeimage-dev \
    libgflags-dev \
    libglew-dev \
    libgoogle-glog-dev \
    libsuitesparse-dev \
    libxi-dev \
    libxmu-dev \
    qt5-default \
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

ENV NCCL_LL_THRESHOLD=0
ENV PYOPENGL_PLATFORM=osmesa

RUN apt-get install -y \
    libzstd-dev \
    libxinerama-dev \
    libxrandr-dev \
    libxcursor-dev \
    libfftw3-dev \
    googletest \
    swig4.0 \
    libxcomp-dev \
    libtiff-dev \
    libassimp-dev \
    libzthread-dev \
    libgtest-dev \
    libglfw3 \
    libglfw3-dev \
    libxcomposite-dev \
    libx11-dev \
    libxxf86vm-dev \
    libxorg-gtest-dev \
    glew-utils \
    libglewmx-dev \
    libgl-dev \
    libosmesa-dev \
    libtiff5-dev \
    libglvnd0 \
    libgl1 \
    libglx0 \
    libegl1 \
    libxext6 \
    libx11-6 

RUN wget -qO- "https://cmake.org/files/v3.21/cmake-3.21.1-linux-x86_64.tar.gz" | tar --strip-components=1 -xz -C /usr/local

WORKDIR /angiogen
COPY requirements.txt .

# Install pip dependancy.
RUN pip install -r requirements.txt

WORKDIR /external/gvxr
RUN mkdir src && mkdir bin && mkdir install
COPY external/gvirtualxray src

ENV GVXR_INSTALL_DIR=/external/gvxr/install

WORKDIR /external/gvxr/bin
RUN cmake \
        -DCMAKE_BUILD_TYPE:STRING=Release \
        -DCMAKE_INSTALL_PREFIX:STRING=$GVXR_INSTALL_DIR \
        -DBUILD_TESTING=OFF \
        -DBUILD_WRAPPER_PYTHON3=ON \
        -DUSE_SYSTEM_ASSIMP=ON \
        -DUSE_SYSTEM_GLEW=ON \
        -DUSE_SYSTEM_GLFW=ON \
        -DUSE_SYSTEM_GTEST=ON \
        -DUSE_SYSTEM_LIBTIFF=ON \
        -S /external/gvxr/src \
        -B $PWD

RUN make -j32

ENV EGL_PLATFORM=x11

ENV NVIDIA_VISIBLE_DEVICES=all
ENV NVIDIA_DRIVER_CAPABILITIES=graphics,utility,compute

WORKDIR /angiogen
RUN cp /external/gvxr/install/gvxrWrapper-1.0.1/python3/* .

RUN apt-get install -y xvfb
RUN pip install scikit-image tqdm fastapi "uvicorn[standard]"

COPY angiogen .

CMD xvfb-run uvicorn server:app --host 0.0.0.0 --port 80
