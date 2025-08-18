#include <oqs/oqs.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    OQS_KEM *kem = NULL;
    uint8_t *public_key = NULL;
    uint8_t *secret_key = NULL;
    FILE *pub_file = NULL;
    FILE *priv_file = NULL;
    OQS_STATUS rc;

    // Initialize ML-KEM-512 (standardized name for Kyber-512)
    kem = OQS_KEM_new(OQS_KEM_alg_ml_kem_512);
    if (kem == NULL) {
        fprintf(stderr, "ERROR: OQS_KEM_new failed for ML-KEM-512\n");
        return 1;
    }

    // Allocate memory for keys
    public_key = malloc(kem->length_public_key);
    secret_key = malloc(kem->length_secret_key);
    if (public_key == NULL || secret_key == NULL) {
        fprintf(stderr, "ERROR: Memory allocation failed\n");
        OQS_KEM_free(kem);
        free(public_key);
        free(secret_key);
        return 1;
    }

    // Generate key pair
    rc = OQS_KEM_keypair(kem, public_key, secret_key);
    if (rc != OQS_SUCCESS) {
        fprintf(stderr, "ERROR: OQS_KEM_keypair failed\n");
        OQS_KEM_free(kem);
        free(public_key);
        free(secret_key);
        return 1;
    }

    // Save public key
    pub_file = fopen("kyber512.pub", "wb");
    if (pub_file == NULL) {
        fprintf(stderr, "ERROR: Unable to open kyber512.pub\n");
        OQS_KEM_free(kem);
        free(public_key);
        free(secret_key);
        return 1;
    }
    if (fwrite(public_key, kem->length_public_key, 1, pub_file) != 1) {
        fprintf(stderr, "ERROR: Unable to write kyber512.pub\n");
        fclose(pub_file);
        OQS_KEM_free(kem);
        free(public_key);
        free(secret_key);
        return 1;
    }
    fclose(pub_file);

    // Save private key
    priv_file = fopen("kyber512.priv", "wb");
    if (priv_file == NULL) {
        fprintf(stderr, "ERROR: Unable to open kyber512.priv\n");
        OQS_KEM_free(kem);
        free(public_key);
        free(secret_key);
        return 1;
    }
    if (fwrite(secret_key, kem->length_secret_key, 1, priv_file) != 1) {
        fprintf(stderr, "ERROR: Unable to write kyber512.priv\n");
        fclose(priv_file);
        OQS_KEM_free(kem);
        free(public_key);
        free(secret_key);
        return 1;
    }
    fclose(priv_file);

    // Success
    printf("Kyber-512 key pair generated: kyber512.pub, kyber512.priv\n");

    // Cleanup
    OQS_KEM_free(kem);
    free(public_key);
    free(secret_key);

    return 0;
}
