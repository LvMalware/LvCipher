# LvCipher

> My own block cipher algorithm based on a Feistel Network

This is an EXPERIMENTAL block cipher algorithm that I created using the symmetric structure known as [Feistel network](https://en.wikipedia.org/wiki/Feistel_cipher).

Please, keep in mind that this cipher was created as a research project and is not supposed to be a bullet-proof encryption algorithm. In fact, the security of this cipher is dubious at best, as it was not 
tested with any cryptographic attack methods as of yet and there is no guarantee that it won't be broken.

Although Feistel networks are proven safe even with a small number of rounds, the security of such an algorithm depends on two key factors: First, the key expansion algorithm must ensure that a given key will always generate the same sequence of subkeys and that no two different keys can generate the same sequence. And secondly, the round function must guarantee the two basic principles of Shannon's entropy: confusion and diffusion.

Confusion means that every bit of the cipher text depends on several bits (if not all) of the key, at the same time that no one can say the dependence between the two. Diffusion means that the correlation between the cipher text and the plain text is so that if we change a single bit in the plain text, aproximately half of the bits of the cipher text should change as well (that is statistically speaking).

If the round function can generate a cryptographically secure pseudorandom output for each round, while ensuring both confusion and difusion with the input, we can be sure that such algorithm would be very secure even against the most advanced attacks.

The problem is that getting both confusion and diffusion at the same time and at an ideal level is a very complicated task and even famous hashing algorithms ~~MD5~~ have already suffered with problems because of this. So if you decide to use this algorithm to encrypt something, don't come to blame me later for any problem that arises when someone can easily break the encryption and have access to your private data... 


## Features of this algorithm


- An entirely new key expansion algorithm (as far as I know)
- The encryption/decryption process uses a variable number of rounds depending on the key
- The round function consists on a small Merkle-Damgard based hashing algorithm with no weird constants to confuse you
- Block size is 32 bytes (256 bits)
- You can use keys up to 256 bytes (bigger keys can be used as well, but only the first 256 bytes will add entropy to the expanded subkeys, while the remainning bytes will have a smaller effect on the process)
- The average encryption rate is 2.7 MB/s (I tried to improve the performance, but maybe it didn't work out too well)
- There is main.c file that is a demonstration of the algorithm. It can be used to encrypt and decrypt files in ECB mode

## Meta

Lucas V. Araujo – lucas.vieira.ar@disroot.org

Distributed under the GNU GPL-3.0+ license. See ``LICENSE`` for more information.

[https://github.com/LvMalware/LvCipher](https://github.com/LvMalware/)

## Contributing

1. Fork it (<https://github.com/LvMalware/LvCipher/fork>)
2. Create your feature branch (`git checkout -b feature/fooBar`)
3. Commit your changes (`git commit -am 'Add some fooBar'`)
4. Push to the branch (`git push origin feature/fooBar`)
5. Create a new Pull Request

## If you are interested in helping improve the algorithm, feel free to make changes or give suggestions.
## Cryptoanalysis is very welcome...