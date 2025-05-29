FROM scratch
COPY cpu_intensive /
ENTRYPOINT ["/cpu_intensive"]
