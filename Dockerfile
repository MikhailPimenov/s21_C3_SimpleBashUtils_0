FROM alpine

RUN apk upgrade && apk add \
    valgrind \
    git \
    make \
    gcc \
    musl-dev \
    bash \
    && rm -rf /var/lib/apt/lists

COPY . project/

WORKDIR /project/src/

RUN addgroup -S appgroup && \
    adduser -S appuser -G appgroup && \
    chmod +x _run_make_valgrind_for_dockerfile.sh

USER appuser
ENTRYPOINT [ "/project/src/_run_make_valgrind_for_dockerfile.sh" ]
