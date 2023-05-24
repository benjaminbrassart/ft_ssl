# ft_ssl

Partial reimplementation of OpenSSL

This is an outer circle projects at 42.
Its goal is to understand and implement some features of OpenSSL.

It is split across 3 sub-projects, each focusing on an aspect of OpenSSL:
- ft_ssl_md5 -> hashing
    - md5
    - sha224 (optional)
    - sha256
    - sha384 (optional)
    - sha512 (optional)
- ft_ssl_des -> cipher/encryption
    - des
    - des-ecb
    - des-cbc
    - des-ofb (optional)
    - des3 (optional)
    - base64
- ft_ssl_rsa -> public key encryption/decryption
    - genrsa
    - rsa
    - rsautl
    - gendsa (optional)
    - gendes (optional)

## Milestones

- ft_ssl_md5 **_ongoing_**
    - [x] md5
    - [ ] sha224
    - [ ] sha256
    - [ ] sha384
    - [ ] sha512
- ft_ssl_des ***maybe later***
- ft_ssl_rsa ***maybe later***

## Building

```sh
git clone --recursive https://github.com/benjaminbrassart/ft_ssl.git ft_ssl
cd ft_ssl
make
```
