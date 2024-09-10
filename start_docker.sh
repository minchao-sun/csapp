# init
# docker run -it \
# --platform linux/amd64 \
# -v ".:/csapp" \
# --name=csapp \
# ubuntu /bin/bash

docker run -it \
--platform linux/amd64 \
-v ".:/csapp" \
--name=csapp \
csapp_image /bin/bash
