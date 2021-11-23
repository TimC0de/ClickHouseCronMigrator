# Using gcc:latest image
FROM gcc:latest as build

COPY . /cronapp
WORKDIR /cronapp

# Install Boost
RUN sh ./boost_install.sh

# Install dependencies
RUN apt-get update && apt-get install -y \
      cmake

# Run CMAKE
RUN if [ -d build ] ; then rm -r build ; fi ; mkdir build
WORKDIR /cronapp/build
RUN cmake -DCMAKE_INSTALL_PREFIX=/cronapp/bin ..

# Compiling
RUN make -j $(nproc)
RUN cmake --install .

# Set workdir to entrypoint
WORKDIR /cronapp/bin

# Setting entrypoint
CMD ["./ClickHouseCronMigrator"]