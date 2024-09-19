using System;
using System.Security.Cryptography;
using System.Text;
using System.IO;

namespace ExcelEncryption
{
    public class EncryptionService
    {
        private Aes aesAlg;
        private RSA rsaAlg;

        public EncryptionService()
        {
            // Initialize AES and RSA algorithms
            aesAlg = Aes.Create();
            rsaAlg = RSA.Create();
        }

        public string EncryptString(string plainText, byte[] key, byte[] iv)
        {
            try
            {
                // Create an AES encryptor using the provided key and IV
                using (var encryptor = aesAlg.CreateEncryptor(key, iv))
                {
                    // Create a MemoryStream to hold the encrypted data
                    using (var msEncrypt = new MemoryStream())
                    {
                        // Create a CryptoStream using the MemoryStream and encryptor
                        using (var csEncrypt = new CryptoStream(msEncrypt, encryptor, CryptoStreamMode.Write))
                        {
                            // Write the plainText to the CryptoStream
                            using (var swEncrypt = new StreamWriter(csEncrypt))
                            {
                                swEncrypt.Write(plainText);
                            }

                            // Convert the encrypted bytes to a Base64 string
                            var encryptedBytes = msEncrypt.ToArray();
                            return Convert.ToBase64String(encryptedBytes);
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                // TODO: Implement proper error handling for encryption failures
                throw new CryptographicException("Encryption failed", ex);
            }
        }

        public string DecryptString(string cipherText, byte[] key, byte[] iv)
        {
            try
            {
                // Convert the Base64 cipherText to bytes
                byte[] cipherBytes = Convert.FromBase64String(cipherText);

                // Create an AES decryptor using the provided key and IV
                using (var decryptor = aesAlg.CreateDecryptor(key, iv))
                {
                    // Create a MemoryStream with the cipherText bytes
                    using (var msDecrypt = new MemoryStream(cipherBytes))
                    {
                        // Create a CryptoStream using the MemoryStream and decryptor
                        using (var csDecrypt = new CryptoStream(msDecrypt, decryptor, CryptoStreamMode.Read))
                        {
                            // Read the decrypted bytes from the CryptoStream
                            using (var srDecrypt = new StreamReader(csDecrypt))
                            {
                                // Convert the decrypted bytes to a string and return
                                return srDecrypt.ReadToEnd();
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                // TODO: Implement proper error handling for decryption failures
                throw new CryptographicException("Decryption failed", ex);
            }
        }

        public void EncryptFile(string inputFile, string outputFile, byte[] key, byte[] iv)
        {
            try
            {
                // Create an AES encryptor using the provided key and IV
                using (var encryptor = aesAlg.CreateEncryptor(key, iv))
                {
                    // Open the input file for reading
                    using (var fsInput = new FileStream(inputFile, FileMode.Open, FileAccess.Read))
                    {
                        // Create the output file for writing
                        using (var fsOutput = new FileStream(outputFile, FileMode.Create, FileAccess.Write))
                        {
                            // Create a CryptoStream using the output FileStream and encryptor
                            using (var csEncrypt = new CryptoStream(fsOutput, encryptor, CryptoStreamMode.Write))
                            {
                                // Copy the input file contents to the CryptoStream
                                fsInput.CopyTo(csEncrypt);
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                // TODO: Implement proper error handling for file encryption failures
                throw new CryptographicException("File encryption failed", ex);
            }
        }
    }
}

// TODO: Human tasks
// - Implement key management and secure key storage
// - Add support for asymmetric encryption using RSA
// - Implement digital signature functionality
// - Add support for different key sizes and encryption strengths
// - Implement secure random number generation for IVs and keys
// - Add support for encrypting and decrypting streams for large data sets
// - Implement key rotation and versioning
// - Add comprehensive logging for encryption and decryption operations
// - Implement performance optimizations for bulk encryption operations
// - Add support for hardware-based encryption (e.g., TPM) where available
// - Implement proper error handling for encryption failures
// - Add support for different encryption modes (e.g., CBC, GCM)
// - Implement proper error handling for decryption failures
// - Add support for integrity verification (e.g., HMAC)
// - Implement progress reporting for large file encryption
// - Add support for encrypting file metadata