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
    chmod +x _run_make_valgrind_for_dockerfile.sh

USER appuser

ENTRYPOINT [ "/project/src/_run_make_valgrind_for_dockerfile.sh" ]
