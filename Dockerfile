FROM ubuntu

RUN apt-get update && apt-get install -y \
    valgrind \
    git \
    make \
    gcc \
    && rm -rf /var/lib/apt/lists

COPY . project/

WORKDIR /project/src/

RUN groupadd -g 999 appuser && \
    useradd -r -u 999 -g appuser appuser && \
    chmod +x run_valgrind_docker.sh

USER appuser

# ENTRYPOINT [ "make", "-f", "C3_SimpleBashUtils-4/src/Makefile", "valgrind" ]
ENTRYPOINT [ "/project/src/run_valgrind_docker.sh" ]
# ENTRYPOINT [ "../bin/bash" ]
# CMD [ "bin/bash" ]


