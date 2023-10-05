
# TASE

TASE scans and retrieves information from game servers using the Valve and GamespyV1 protocols. It collects details such as map, server name, players, and more. Additionally, it provides geolocalization information for the servers it scans. The information is written into an output JSON file.

## Features

- Multithreaded design for concurrent scanning of multiple servers.
- Support for both the Valve and GamespyV1 protocols (UDP).
- Geolocalization of game servers using a local SQLite3 database and external geolocalization API.
- Generates an output JSON file containing game server information and geolocalization data.

## Game server scanning
TASE constructs and sends UDP datagrams containing specific protocol-specific queries to the game servers' IP addresses and ports. The scanner parses the responses according to the respective protocol specifications to extract and process the server information.

## Geolocalization
TASE uses a two-way scheme for geolocalization:

1.  **Local SQLite3 Database**: Queries a local SQLite3 database by server IP to check if geolocalization data already exists.
    
2.  **External Geolocalization API**: If geolocalization information is not found locally, the scanner sends a TCP/HTTP GET request to a free geolocalization API to retrieve server geolocalization information. The local SQLite3 database is updated with this information to avoid repeated API requests.
