```bash
sudo wget https://github.com/openssl/openssl/releases/download/openssl-3.4.0/openssl-3.4.0.tar.gz

./config --prefix=/opt/openssl-3.0 --openssldir=/opt/openssl-3.0 shared

make -j$(nproc)

sudo make install
```

boost 1.79