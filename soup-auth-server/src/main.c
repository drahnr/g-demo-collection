/*
 * Author:
 *  Bernhard Schuster <schuster.bernhard@gmail.com>
 *
 * Web page: https://srctwig.com/
 *
 * Copyright (C) 2013  Bernhard Schuster
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */
 
 
#include <stdlib.h>

#include <glib.h>
#include <libsoup/soup.h>

//self signed dummy key/cert
const gchar pem_data[] = "-----BEGIN RSA PRIVATE KEY-----"
"MIICXQIBAAKBgQDT1O1ElYwZ0EvIgZnH0T5qdyZp/m8TPQVH8rnqTiffTjGtKUll"
"TNsEbGyLx1MdCSdYfKLEgIgUsz9YAZLWlr53hd5ANk27Wdya+t3YqCUNfaClOKVF"
"edPc/lp2epfzj+mkV1V6E0eTFCe2+y/tPUQ6QyFmWfPUo+vOAi2ShGVLawIDAQAB"
"AoGBAIxdEQNbHMSWE0E/L0kKS5N8dEXTxII48uDj43FwbFc5lw0oyGuJa68YsSxF"
"3brlr96gZ7JRgNpGk/iawlPS+00+6Yq6w8sMve+yyXL4JktZgxQSCtSmn0DReiJ7"
"r7gnpq0M5xLE7FnsM0bUrTToUappyI6QBHizO+6E4rqF3/5BAkEA8P/OOpTQpYdn"
"jMGoYLnl1PDQ8vsEUFWoZzQiGbMwEV8KP/oy2MX8SYiD2EwfWFXkmBR/uXYZ3/wK"
"iBtLZQsIewJBAOEEWk6Lkqn/siuSR8d21IQKW0lcosCnxOLTQZwONQCC6RyIHkH5"
"fXaD+FzQk7NlqJ2CcmVhgu3Jt9jPUy4lbdECQDKgGetWbYoIKql32O4QvwX/XHVe"
"Y6aYh9q+KdeqO5O6mkWcQmglFv0/byrVutPsnyuPStFjzMWvhWrYCPt92NcCQGvY"
"/TuWM+Zc0lWF1/dnIfZjTFt+FdeG0ZvxqwmT2Z10ISx2pSHz5UlPeVNq+M/MEqPj"
"qNakhO9Zdk9pFzIP6HECQQDL9a0I/UynC27trAUBkUd48VmUxEEnufUx7xGhK1DH"
"bngngH84dFPcIROYkPe35JTtfZk0GpEzZkQ/yrK7Y5jG"
"-----END RSA PRIVATE KEY-----"
"-----BEGIN CERTIFICATE-----"
"MIICATCCAWoCCQCh0/H/l7u2qjANBgkqhkiG9w0BAQUFADBFMQswCQYDVQQGEwJB"
"VTETMBEGA1UECAwKU29tZS1TdGF0ZTEhMB8GA1UECgwYSW50ZXJuZXQgV2lkZ2l0"
"cyBQdHkgTHRkMB4XDTEzMTAxMDAxMzMzNFoXDTIzMTAwODAxMzMzNFowRTELMAkG"
"A1UEBhMCQVUxEzARBgNVBAgMClNvbWUtU3RhdGUxITAfBgNVBAoMGEludGVybmV0"
"IFdpZGdpdHMgUHR5IEx0ZDCBnzANBgkqhkiG9w0BAQEFAAOBjQAwgYkCgYEA09Tt"
"RJWMGdBLyIGZx9E+ancmaf5vEz0FR/K56k4n304xrSlJZUzbBGxsi8dTHQknWHyi"
"xICIFLM/WAGS1pa+d4XeQDZNu1ncmvrd2KglDX2gpTilRXnT3P5adnqX84/ppFdV"
"ehNHkxQntvsv7T1EOkMhZlnz1KPrzgItkoRlS2sCAwEAATANBgkqhkiG9w0BAQUF"
"AAOBgQBX3ixk/rX4Bs5GMiOhFl0Uf9Cmoeb28BiisWsICTXZGeacc2hmQdUZYZAq"
"B2jbVWcLyUnA2FMIdpiwr2eGhEDnn4hdXylMqC5lBxfmJGxnJN0fDTBPw6DUE3Ko"
"hzKRTqmywcPY58jq4DG48O8UkQjhgaVbSLgV1Xr11q2Ia8MZfg=="
"-----END CERTIFICATE-----";

static void
server_callback (SoupServer        *server,
                 SoupMessage       *msg, 
                 const char        *path,
                 GHashTable        *query,
                 SoupClientContext *client,
                 gpointer           user_data)
{
	//just for the fun of it, print some stuff
	{
		SoupURI *uri = soup_message_get_uri (msg); // do not deref, still owned by msg
		g_message ("scheme  \t%s", uri->scheme);
		g_message ("user    \t%s", uri->user);
		g_message ("passwd  \t%s", uri->password);
		g_message ("host    \t%s", uri->host);
		g_message ("port    \t%u", uri->port);
		g_message ("path    \t%s", uri->path);
		g_message ("query   \t%s", uri->query);
		g_message ("fragment\t%s", uri->fragment);
	}
}


gchar *
auth_callback (SoupAuthDomain *domain, SoupMessage *msg, const char *username, gpointer user_data)
{
	GHashTable *map = user_data;
	gchar *digest = g_hash_table_lookup (map, username);
	g_message ("Auth request for %s ... %s", username, (gchar *)digest);
	return g_strdup (digest);
}

void
register_user (GHashTable *map, const gchar *user, const gchar *realm, const gchar *passwd)
{
	g_assert (user);
	gchar *digest = soup_auth_domain_digest_encode_password(user, realm, passwd);
	g_assert (digest);
	g_hash_table_insert (map, (gchar *)user, digest);
}

int main (int argc, char *argv[])
{
	GError *error = NULL;
	GHashTable *map;
	GTlsCertificate *cert;
	GMainLoop *loop;
	SoupAuthDomain *authdomain = NULL;
	SoupServer *server;

	const gchar realm[] = "unicornrainbow";


	map = g_hash_table_new_full (g_str_hash, g_str_equal, NULL, g_free);
	register_user (map, "joe", realm, "abc123");
	register_user (map, "john", realm, "doe");
	register_user (map, "jack", realm, "black");


	cert = g_tls_certificate_new_from_pem (pem_data, -1, &error);

	if (error) {
		g_warning ("GTlsCert issue: %i - %s", error->code, error->message);
		g_error_free (error);
		return EXIT_FAILURE;
	}

	server = soup_server_new (SOUP_SERVER_PORT, 9009,
			                      SOUP_SERVER_SERVER_HEADER, "voodooserv",
			                      SOUP_SERVER_TLS_CERTIFICATE, cert,
			                      NULL);
	g_assert (server);

	authdomain = soup_auth_domain_digest_new (
		                 SOUP_AUTH_DOMAIN_REALM, realm, /*defined in accessstore.h*/
			             SOUP_AUTH_DOMAIN_DIGEST_AUTH_CALLBACK, auth_callback,
			             SOUP_AUTH_DOMAIN_DIGEST_AUTH_DATA, map,
			             NULL);
	soup_auth_domain_add_path (authdomain, "/"); //make everything requering auth

	soup_server_add_auth_domain (server, SOUP_AUTH_DOMAIN (authdomain));

	g_object_unref (authdomain);

	soup_server_add_handler (server,
		                     NULL /* == all/being default handler */,
		                     server_callback,
		                     NULL,
		                     NULL);

	g_assert (soup_server_is_https (server));

	soup_server_run_async (server);

	g_message ("Server up and running... waiting for requests");
	
	
	loop = g_main_loop_new (NULL, FALSE);
	g_main_loop_run (loop);
	g_main_loop_unref (loop);
	
	soup_server_quit (server);
	g_object_unref (cert);
	
	g_hash_table_unref (map);

	return EXIT_SUCCESS;
}
