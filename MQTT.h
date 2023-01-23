/**
 * @file   MQTT.h
 * @author Jacob Corr
 * @date   22/01/2023
 * @brief  Header file containing functions that allow for MQTT connection 
 *         with Google Cloud Platform
 */

#ifndef __MQTT_H__
#define __MQTT_H__

#define MQTTCLIENT_QOS1                 0
#define MQTTCLIENT_QOS2                 0
#define MQTT_MAX_CONNECTIONS            5
#define MQTT_MAX_PACKET_SIZE            1024
#define TIME_JWT_EXP                    (60*60*24)
#define JWT_BUF_SIZE                    1024

const char* PROJ_ID                     = "mbed-to-gcp";
const char* REGION                      = "europe-west1";
const char* REGISTRY                    = "iot_registry";
const char* DEV_ID                      = "K64F";
const char* TOPIC                       = "K64F-sub";
const char  MQTT_SERVER_HOST_NAME[]     = "mqtt.googleapis.com";
const int   MQTT_SERVER_PORT            = 8883;

/* Root Certificates */
const char* SSL_CA_PEM =
"# Operating CA: GlobalSign\n"
"# Issuer: CN=GlobalSign Root CA O=GlobalSign nv-sa OU=Root CA\n"
"# Subject: CN=GlobalSign Root CA O=GlobalSign nv-sa OU=Root CA\n"
"# Label: \"GlobalSign Root CA\"\n"
"# Serial: 4835703278459707669005204\n"
"# MD5 Fingerprint: 3e:45:52:15:09:51:92:e1:b7:5d:37:9f:b1:87:29:8a\n"
"# SHA1 Fingerprint: b1:bc:96:8b:d4:f4:9d:62:2a:a8:9a:81:f2:15:01:52:a4:1d:82:9c\n"
"# SHA256 Fingerprint: eb:d4:10:40:e4:bb:3e:c7:42:c9:e3:81:d3:1e:f2:a4:1a:48:b6:68:5c:96:e7:ce:f3:c1:df:6c:d4:33:1c:99\n"
"-----BEGIN CERTIFICATE-----\n"
"MIIDdTCCAl2gAwIBAgILBAAAAAABFUtaw5QwDQYJKoZIhvcNAQEFBQAwVzELMAkG\n"
"A1UEBhMCQkUxGTAXBgNVBAoTEEdsb2JhbFNpZ24gbnYtc2ExEDAOBgNVBAsTB1Jv\n"
"b3QgQ0ExGzAZBgNVBAMTEkdsb2JhbFNpZ24gUm9vdCBDQTAeFw05ODA5MDExMjAw\n"
"MDBaFw0yODAxMjgxMjAwMDBaMFcxCzAJBgNVBAYTAkJFMRkwFwYDVQQKExBHbG9i\n"
"YWxTaWduIG52LXNhMRAwDgYDVQQLEwdSb290IENBMRswGQYDVQQDExJHbG9iYWxT\n"
"aWduIFJvb3QgQ0EwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDaDuaZ\n"
"jc6j40+Kfvvxi4Mla+pIH/EqsLmVEQS98GPR4mdmzxzdzxtIK+6NiY6arymAZavp\n"
"xy0Sy6scTHAHoT0KMM0VjU/43dSMUBUc71DuxC73/OlS8pF94G3VNTCOXkNz8kHp\n"
"1Wrjsok6Vjk4bwY8iGlbKk3Fp1S4bInMm/k8yuX9ifUSPJJ4ltbcdG6TRGHRjcdG\n"
"snUOhugZitVtbNV4FpWi6cgKOOvyJBNPc1STE4U6G7weNLWLBYy5d4ux2x8gkasJ\n"
"U26Qzns3dLlwR5EiUWMWea6xrkEmCMgZK9FGqkjWZCrXgzT/LCrBbBlDSgeF59N8\n"
"9iFo7+ryUp9/k5DPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNVHRMBAf8E\n"
"BTADAQH/MB0GA1UdDgQWBBRge2YaRQ2XyolQL30EzTSo//z9SzANBgkqhkiG9w0B\n"
"AQUFAAOCAQEA1nPnfE920I2/7LqivjTFKDK1fPxsnCwrvQmeU79rXqoRSLblCKOz\n"
"yj1hTdNGCbM+w6DjY1Ub8rrvrTnhQ7k4o+YviiY776BQVvnGCv04zcQLcFGUl5gE\n"
"38NflNUVyRRBnMRddWQVDf9VMOyGj/8N7yy5Y0b2qvzfvGn9LhJIZJrglfCm7ymP\n"
"AbEVtQwdpf5pLGkkeB6zpxxxYu7KyJesF12KwvhHhm4qxFYxldBniYUr+WymXUad\n"
"DKqC5JlR3XC321Y9YeRq4VzW9v493kHMB65jUr9TU/Qr6cf9tveCX4XSQRjbgbME\n"
"HMUfpIBvFSDJ3gyICh3WZlXi/EjJKSZp4A==\n"
"-----END CERTIFICATE-----\n"
"\n"
"# Operating CA: GlobalSign\n"
"# Issuer: CN=GlobalSign O=GlobalSign OU=GlobalSign Root CA - R2\n"
"# Subject: CN=GlobalSign O=GlobalSign OU=GlobalSign Root CA - R2\n"
"# Label: \"GlobalSign Root CA - R2\"\n"
"# Serial: 4835703278459682885658125\n"
"# MD5 Fingerprint: 94:14:77:7e:3e:5e:fd:8f:30:bd:41:b0:cf:e7:d0:30\n"
"# SHA1 Fingerprint: 75:e0:ab:b6:13:85:12:27:1c:04:f8:5f:dd:de:38:e4:b7:24:2e:fe\n"
"# SHA256 Fingerprint: ca:42:dd:41:74:5f:d0:b8:1e:b9:02:36:2c:f9:d8:bf:71:9d:a1:bd:1b:1e:fc:94:6f:5b:4c:99:f4:2c:1b:9e\n"
"-----BEGIN CERTIFICATE-----\n"
"MIIDujCCAqKgAwIBAgILBAAAAAABD4Ym5g0wDQYJKoZIhvcNAQEFBQAwTDEgMB4G\n"
"A1UECxMXR2xvYmFsU2lnbiBSb290IENBIC0gUjIxEzARBgNVBAoTCkdsb2JhbFNp\n"
"Z24xEzARBgNVBAMTCkdsb2JhbFNpZ24wHhcNMDYxMjE1MDgwMDAwWhcNMjExMjE1\n"
"MDgwMDAwWjBMMSAwHgYDVQQLExdHbG9iYWxTaWduIFJvb3QgQ0EgLSBSMjETMBEG\n"
"A1UEChMKR2xvYmFsU2lnbjETMBEGA1UEAxMKR2xvYmFsU2lnbjCCASIwDQYJKoZI\n"
"hvcNAQEBBQADggEPADCCAQoCggEBAKbPJA6+Lm8omUVCxKs+IVSbC9N/hHD6ErPL\n"
"v4dfxn+G07IwXNb9rfF73OX4YJYJkhD10FPe+3t+c4isUoh7SqbKSaZeqKeMWhG8\n"
"eoLrvozps6yWJQeXSpkqBy+0Hne/ig+1AnwblrjFuTosvNYSuetZfeLQBoZfXklq\n"
"tTleiDTsvHgMCJiEbKjNS7SgfQx5TfC4LcshytVsW33hoCmEofnTlEnLJGKRILzd\n"
"C9XZzPnqJworc5HGnRusyMvo4KD0L5CLTfuwNhv2GXqF4G3yYROIXJ/gkwpRl4pa\n"
"zq+r1feqCapgvdzZX99yqWATXgAByUr6P6TqBwMhAo6CygPCm48CAwEAAaOBnDCB\n"
"mTAOBgNVHQ8BAf8EBAMCAQYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUm+IH\n"
"V2ccHsBqBt5ZtJot39wZhi4wNgYDVR0fBC8wLTAroCmgJ4YlaHR0cDovL2NybC5n\n"
"bG9iYWxzaWduLm5ldC9yb290LXIyLmNybDAfBgNVHSMEGDAWgBSb4gdXZxwewGoG\n"
"3lm0mi3f3BmGLjANBgkqhkiG9w0BAQUFAAOCAQEAmYFThxxol4aR7OBKuEQLq4Gs\n"
"J0/WwbgcQ3izDJr86iw8bmEbTUsp9Z8FHSbBuOmDAGJFtqkIk7mpM0sYmsL4h4hO\n"
"291xNBrBVNpGP+DTKqttVCL1OmLNIG+6KYnX3ZHu01yiPqFbQfXf5WRDLenVOavS\n"
"ot+3i9DAgBkcRcAtjOj4LaR0VknFBbVPFd5uRHg5h6h+u/N5GJG79G+dwfCMNYxd\n"
"AfvDbbnvRG15RjF+Cv6pgsH/76tuIMRQyV+dTZsXjAzlAcmgQWpzU/qlULRuJQ/7\n"
"TBj0/VLZjmmx6BEP3ojY+x1J96relc8geMJgEtslQIxq/H5COEBkEveegeGTLg==\n"
"-----END CERTIFICATE-----\n"
"\n"
"# Operating CA: GlobalSign\n"
"# Issuer: CN=GlobalSign O=GlobalSign OU=GlobalSign Root CA - R3\n"
"# Subject: CN=GlobalSign O=GlobalSign OU=GlobalSign Root CA - R3\n"
"# Label: \"GlobalSign Root CA - R3\"\n"
"# Serial: 4835703278459759426209954\n"
"# MD5 Fingerprint: c5:df:b8:49:ca:05:13:55:ee:2d:ba:1a:c3:3e:b0:28\n"
"# SHA1 Fingerprint: d6:9b:56:11:48:f0:1c:77:c5:45:78:c1:09:26:df:5b:85:69:76:ad\n"
"# SHA256 Fingerprint: cb:b5:22:d7:b7:f1:27:ad:6a:01:13:86:5b:df:1c:d4:10:2e:7d:07:59:af:63:5a:7c:f4:72:0d:c9:63:c5:3b\n"
"-----BEGIN CERTIFICATE-----\n"
"MIIDXzCCAkegAwIBAgILBAAAAAABIVhTCKIwDQYJKoZIhvcNAQELBQAwTDEgMB4G\n"
"A1UECxMXR2xvYmFsU2lnbiBSb290IENBIC0gUjMxEzARBgNVBAoTCkdsb2JhbFNp\n"
"Z24xEzARBgNVBAMTCkdsb2JhbFNpZ24wHhcNMDkwMzE4MTAwMDAwWhcNMjkwMzE4\n"
"MTAwMDAwWjBMMSAwHgYDVQQLExdHbG9iYWxTaWduIFJvb3QgQ0EgLSBSMzETMBEG\n"
"A1UEChMKR2xvYmFsU2lnbjETMBEGA1UEAxMKR2xvYmFsU2lnbjCCASIwDQYJKoZI\n"
"hvcNAQEBBQADggEPADCCAQoCggEBAMwldpB5BngiFvXAg7aEyiie/QV2EcWtiHL8\n"
"RgJDx7KKnQRfJMsuS+FggkbhUqsMgUdwbN1k0ev1LKMPgj0MK66X17YUhhB5uzsT\n"
"gHeMCOFJ0mpiLx9e+pZo34knlTifBtc+ycsmWQ1z3rDI6SYOgxXG71uL0gRgykmm\n"
"KPZpO/bLyCiR5Z2KYVc3rHQU3HTgOu5yLy6c+9C7v/U9AOEGM+iCK65TpjoWc4zd\n"
"QQ4gOsC0p6Hpsk+QLjJg6VfLuQSSaGjlOCZgdbKfd/+RFO+uIEn8rUAVSNECMWEZ\n"
"XriX7613t2Saer9fwRPvm2L7DWzgVGkWqQPabumDk3F2xmmFghcCAwEAAaNCMEAw\n"
"DgYDVR0PAQH/BAQDAgEGMA8GA1UdEwEB/wQFMAMBAf8wHQYDVR0OBBYEFI/wS3+o\n"
"LkUkrk1Q+mOai97i3Ru8MA0GCSqGSIb3DQEBCwUAA4IBAQBLQNvAUKr+yAzv95ZU\n"
"RUm7lgAJQayzE4aGKAczymvmdLm6AC2upArT9fHxD4q/c2dKg8dEe3jgr25sbwMp\n"
"jjM5RcOO5LlXbKr8EpbsU8Yt5CRsuZRj+9xTaGdWPoO4zzUhw8lo/s7awlOqzJCK\n"
"6fBdRoyV3XpYKBovHd7NADdBj+1EbddTKJd+82cEHhXXipa0095MJ6RMG3NzdvQX\n"
"mcIfeg7jLQitChws/zyrVQ4PkX4268NXSb7hLi18YIvDQVETI53O9zJrlAGomecs\n"
"Mx86OyXShkDOOyyGeMlhLxS67ttVb9+E7gUJTb0o2HLO02JQZR7rkpeDMdmztcpH\n"
"WD9f\n"
"-----END CERTIFICATE-----\n";

/* Client certificate */
const char* SSL_CLIENT_CERT_PEM = 
"-----BEGIN CERTIFICATE-----\n"
"MIIDZDCCAkwCFGlzAvQnYdSyE8Tg7iYuxqImRJfOMA0GCSqGSIb3DQEBCwUAMG8x\n"
"CzAJBgNVBAYTAkpQMQ4wDAYDVQQIDAVUb2t5bzETMBEGA1UEBwwKQ2hpeW9kYS1r\n"
"dTESMBAGA1UECgwJTXlDb21wYW55MRUwEwYDVQQLDAxNeURlcGFydG1lbnQxEDAO\n"
"BgNVBAMMB1NFTEZfQ0EwHhcNMjMwMTA1MTIyMTI1WhcNMjQwMTA1MTIyMTI1WjBu\n"
"MQswCQYDVQQGEwJKUDEOMAwGA1UECAwFVG9reW8xEzARBgNVBAcMCkNoaXlvZGEt\n"
"a3UxEjAQBgNVBAoMCU15Q29tcGFueTEVMBMGA1UECwwMTXlEZXBhcnRtZW50MQ8w\n"
"DQYDVQQDDAZDTElFTlQwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDN\n"
"fw3z6WY4gyDblqaJLq+hIKSwv+XuOUaZuiqckZMQuG8DdKEiSN6KQogGpH41zDaO\n"
"mJq1mG7UdzPOIDWfKpGZG4uY2CN1db30gvrvZO9tS5gI9jterSJi4WVvNMEM0Vz+\n"
"sgHTcWq8dci7vUDQGVylSoBzep4eCNTK0oTMM4a+gdJQJascdiusa+0u7dIpOP5O\n"
"+78/Xwk6I66Zr421WnuhrTRlwBUzAdTez4CA6gR8kY/iaYB704VLG5fxUZroinUc\n"
"KcHEDMa/inBWqbxla/Mb7iVcFvt3k7ruY//1zpGj/FpfPj93QeeNllfudJsnh2pG\n"
"iNPEULkCynUipJF7599lAgMBAAEwDQYJKoZIhvcNAQELBQADggEBAEVqqxBTNq4Q\n"
"kRzlCvOhmWhpCo6Ou54r/KHn+lz3nsRIfmb8zJ3aWd3iOYXc3OfycLZQpDP4AkCN\n"
"flAfDuVbfElB9o/RJQ5RxF4Km8wi/E+2qpatPV44Y4V/kSH9xTH65jSO0ovxcnY7\n"
"Xs4GwYwcFCrnGOPnLdjc0qdnzyPBbNH5pWG73Jayzoc+sApudUfK78r5pMBCxf6c\n"
"/gW0IAvoGcWCozl/0CDShm0SO0/CozazcNqQvWYcIerO4uck5F7CERjDk6AjLiRZ\n"
"LHT+wYcayJAkGcqcubeT2cizE3XQQJHEqofwWheiqy9g+oXF3901MKmwBaEej590\n"
"ySltpn+o43o=\n"
"-----END CERTIFICATE-----\n";

/* Client private key */
const char* SSL_CLIENT_PRIVATE_KEY_PEM = 
"-----BEGIN RSA PRIVATE KEY-----\n"
"MIIEogIBAAKCAQEAzX8N8+lmOIMg25amiS6voSCksL/l7jlGmboqnJGTELhvA3Sh\n"
"IkjeikKIBqR+Ncw2jpiatZhu1HczziA1nyqRmRuLmNgjdXW99IL672TvbUuYCPY7\n"
"Xq0iYuFlbzTBDNFc/rIB03FqvHXIu71A0BlcpUqAc3qeHgjUytKEzDOGvoHSUCWr\n"
"HHYrrGvtLu3SKTj+Tvu/P18JOiOuma+NtVp7oa00ZcAVMwHU3s+AgOoEfJGP4mmA\n"
"e9OFSxuX8VGa6Ip1HCnBxAzGv4pwVqm8ZWvzG+4lXBb7d5O67mP/9c6Ro/xaXz4/\n"
"d0HnjZZX7nSbJ4dqRojTxFC5Asp1IqSRe+ffZQIDAQABAoIBADi4+3b6U/cBaw5x\n"
"9SIkuuH1CsCnF6a8NJq2YpC5NikFX89KYDgAeT0cxENhIF1lMyIFSZ8apm0LQkeB\n"
"aIu4ylvC2LOl5ZPEnlxh0zZFXDmFBkBjB1xI2hcFNGSP4UHSgKLlK8/bnKTMtk3s\n"
"HXnTplkRyYcWr5KGyYX6uiFLj8QBvvXnBAKM9u1NTQR1oN+f/jnjCW2qqHBU8C2T\n"
"BssFE63g65RZjIniZ4E7dCEAcO5mH5ItXsEvvE4N0lrh/lmikMkMGfuaUF5/jqKg\n"
"IsU/Rb1At2AW0hJ7w9dTNTOaWEZFbeC2/gAWmROtE8SRQ4GQylIath1yly8003wQ\n"
"qDq9sfkCgYEA7+FHY3xjChUsy/6flNDC+M5Ijcb0WaWW0PAXH4G+ytl/HAZValH7\n"
"86nGNKrseDuMis81kW9Pf1xmJK0alSVQLJzD0p+0RVUO1FoNZrann72vaDTEnXbf\n"
"M/qPE++D0TccPmnwAy5GrjArLO/qauMModtwzO+xnQdQ692W1YqvVYsCgYEA205D\n"
"fgeX0p8yTcwpJNiE/vAq1uQbsN5CiQBMbpGh4QZhpHuIp/GJ3WtzBEeXHtns5/mw\n"
"SJ6qRINh3azuS3pc64crP2KNJj2frfTfgY+a0dF1uPtO07l4EUghnY6ws1czF18B\n"
"No13SaKd+sVo7nrRKj+4Gt3lF/AYyXn6ZiojnM8CgYBZxybAxT0L+QDbGGEBgWB3\n"
"Xj0pUt0A8EiRykndMOIHaX95LaWqNSTm/ROBgrceCKuBCCw0UkSeliZkc6UPTPfE\n"
"uqdPnRYLHNp//3QkjRgPYR7VEWrExEilW5FRFMNw+f9IHL4HcrphVwXRu7kwZ+nC\n"
"ai69hLgk3SgcHsLlCRK2IQKBgC8nDbXsVgVwOzWk/vewIjBaXSulEhseProMOWLQ\n"
"pzil0Zq3APPzKAxWjP1PkzuL6bca5X9Hth5TDVjHVNkfrxjpEDBptSWSvGF/tAq8\n"
"YLJaUfB7AEDfsOQuWSPySSXWSuLsh2s+UXago00G4v/boltuJdyI9zgFZJ71PhR3\n"
"XP7bAoGAZNyi89LdSB+ifdMrk0gMZxgwiPnyPLp2dnSXf6uJm+A4zGitO4WuZRbc\n"
"YAI8s1oXFr8Gth8WfPFaeYKtXBkBGoBm5sC/iIffidapKkfou3r3SDqmuuDBv6du\n"
"k6GbQp3elYz0SjMfdHBiyzBBRcSuXZ0pxQyFCPfbwNmujtlViMQ=\n"
"-----END RSA PRIVATE KEY-----\n";

#endif /* __MQTT_H__ */
