/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ed25519/ed25519.h>

#include "cryptography/ed25519_sha3_impl/internal/ed25519_impl.hpp"
#include "cryptography/ed25519_sha3_impl/internal/sha3_hash.hpp"

namespace iroha {

  /**
   * Sign the message
   */
  sig_t sign(const uint8_t *msg,
             size_t msgsize,
             const pubkey_t &pub,
             const privkey_t &priv) {
    sig_t sig;
    ed25519_sign(reinterpret_cast<signature_t *>(sig.data()),
                 msg,
                 msgsize,
                 reinterpret_cast<const public_key_t *>(pub.data()),
                 reinterpret_cast<const private_key_t *>(priv.data()));
    return sig;
  }

  sig_t sign(const std::string &msg,
             const pubkey_t &pub,
             const privkey_t &priv) {
    return sign(
        reinterpret_cast<const uint8_t *>(msg.data()), msg.size(), pub, priv);
  }

  /**
   * Verify signature
   */
  bool verify(const uint8_t *msg,
              size_t msgsize,
              const pubkey_t &pub,
              const sig_t &sig) {
    return 1
        == ed25519_verify(reinterpret_cast<const signature_t *>(sig.data()),
                          msg,
                          msgsize,
                          reinterpret_cast<const public_key_t *>(pub.data()));
  }

  bool verify(const std::string &msg, const pubkey_t &pub, const sig_t &sig) {
    return 1
        == verify(reinterpret_cast<const uint8_t *>(msg.data()),
                  msg.size(),
                  pub,
                  sig);
  }

  /**
   * Generate seed
   */
  blob_t<32> create_seed() {
    blob_t<32> seed;
    randombytes(seed.data(), seed.size());
    return seed;
  }

  /**
   * Generate 32 bytes seed based on a passphrase
   * @param passphrase
   * @return
   */
  blob_t<32> create_seed(std::string passphrase) {
    return sha3_256((uint8_t *)passphrase.data(), passphrase.size());
  }

  /**
   * Create keypair
   */
  keypair_t create_keypair(blob_t<32> seed) {
    keypair_t kp;
    kp.privkey = seed;

    ed25519_derive_public_key(
        reinterpret_cast<const private_key_t *>(kp.privkey.data()),
        reinterpret_cast<public_key_t *>(kp.pubkey.data()));

    return kp;
  }

  keypair_t create_keypair() {
    return create_keypair(create_seed());
  }
}  // namespace iroha
