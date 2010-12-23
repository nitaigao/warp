#include "Multicast.h"

#include <iostream>

static const int BUFFER_SIZE = 64;

static const char* MULTICAST_GROUP = "225.1.2.3";
static const int MULTICAST_PORT = 45515;
static const int MTU = 1500;
static const int TMW_SQNS = 100;
static const int MULTICAST_HOPS = 16;
static const int AMBIENT_SPM = pgm_secs(30);
static const int HEARTBEAT_SPM[] = { 
  pgm_msecs (100),
  pgm_msecs (100),
  pgm_msecs (100),
  pgm_msecs (100),
  pgm_msecs (1300),
  pgm_secs  (7),
  pgm_secs  (16),
  pgm_secs  (25),
  pgm_secs  (30) 
};

static const int PASSIVE = 0;
static const int PEER_EXPIRY = pgm_secs (300);
static const int SPMR_EXPIRY = pgm_secs (250);
static const int NAK_BO_IVL = pgm_msecs (50);
static const int NAK_RPT_IVL = pgm_secs(2);
static const int NAK_RDATA_IVL = pgm_secs (2);
static const int NAK_DATA_RETRIES = 50;
static const int NAK_NCF_RETRIES = 50;
static const int MULTICAST_LOOP = 0;

static std::string hostname()
{
  char hostname[BUFFER_SIZE];
  memset(hostname, 0, BUFFER_SIZE);
  gethostname(hostname, BUFFER_SIZE);
  return std::string(hostname);
}

Multicast::Multicast()
{
  pgm_error_t* pgm_err = NULL;
	sa_family_t sa_family = AF_UNSPEC;
  struct pgm_addrinfo_t* res = NULL;
  
  pgm_messages_init();
	pgm_init (&pgm_err);
  
  if (!pgm_getaddrinfo (MULTICAST_GROUP, NULL, &res, &pgm_err))
  {
    std::cerr << "fail setting mutlicast group" << std::endl; 
  }
  
  sa_family = res->ai_send_addrs[0].gsr_group.ss_family;

  if (!pgm_socket(&socket_, sa_family, SOCK_SEQPACKET, IPPROTO_UDP, &pgm_err))
  {
    std::cerr << "fail creating socket: " << pgm_err->message << std::endl;
  }
  
  pgm_setsockopt (socket_, IPPROTO_PGM, PGM_UDP_ENCAP_UCAST_PORT, &MULTICAST_PORT, sizeof(MULTICAST_PORT));
  pgm_setsockopt (socket_, IPPROTO_PGM, PGM_UDP_ENCAP_MCAST_PORT, &MULTICAST_PORT, sizeof(MULTICAST_PORT));
  
  // COMMON
  pgm_setsockopt(socket_, IPPROTO_PGM, PGM_MTU, &MTU, sizeof(MTU));
  pgm_setsockopt(socket_, IPPROTO_PGM, PGM_MULTICAST_HOPS, &MULTICAST_HOPS, sizeof(MULTICAST_HOPS));
  pgm_setsockopt(socket_, IPPROTO_PGM, PGM_MULTICAST_LOOP, &MULTICAST_LOOP, sizeof(MULTICAST_LOOP));


  // TX
  pgm_setsockopt(socket_, IPPROTO_PGM, PGM_TXW_SQNS, &TMW_SQNS, sizeof(TMW_SQNS));
  pgm_setsockopt(socket_, IPPROTO_PGM, PGM_AMBIENT_SPM, &AMBIENT_SPM, sizeof(AMBIENT_SPM));
  pgm_setsockopt(socket_, IPPROTO_PGM, PGM_HEARTBEAT_SPM, &HEARTBEAT_SPM, sizeof(HEARTBEAT_SPM));
  
  // RX
  pgm_setsockopt (socket_, IPPROTO_PGM, PGM_PASSIVE, &PASSIVE, sizeof(PASSIVE));
	pgm_setsockopt (socket_, IPPROTO_PGM, PGM_RXW_SQNS, &TMW_SQNS, sizeof(TMW_SQNS));
	pgm_setsockopt (socket_, IPPROTO_PGM, PGM_PEER_EXPIRY, &PEER_EXPIRY, sizeof(PEER_EXPIRY));
	pgm_setsockopt (socket_, IPPROTO_PGM, PGM_SPMR_EXPIRY, &SPMR_EXPIRY, sizeof(SPMR_EXPIRY));
	pgm_setsockopt (socket_, IPPROTO_PGM, PGM_NAK_BO_IVL, &NAK_BO_IVL, sizeof(NAK_BO_IVL));
	pgm_setsockopt (socket_, IPPROTO_PGM, PGM_NAK_RPT_IVL, &NAK_RPT_IVL, sizeof(NAK_RPT_IVL));
	pgm_setsockopt (socket_, IPPROTO_PGM, PGM_NAK_RDATA_IVL, &NAK_RDATA_IVL, sizeof(NAK_RDATA_IVL));
	pgm_setsockopt (socket_, IPPROTO_PGM, PGM_NAK_DATA_RETRIES, &NAK_DATA_RETRIES, sizeof(NAK_DATA_RETRIES));
	pgm_setsockopt (socket_, IPPROTO_PGM, PGM_NAK_NCF_RETRIES, &NAK_NCF_RETRIES, sizeof(NAK_NCF_RETRIES));
    
  pgm_setsockopt (socket_, IPPROTO_PGM, PGM_JOIN_GROUP, &res->ai_recv_addrs[0], sizeof(struct group_req));
	pgm_setsockopt (socket_, IPPROTO_PGM, PGM_SEND_GROUP, &res->ai_send_addrs[0], sizeof(struct group_req));
  
  struct pgm_interface_req_t if_req;
	memset (&if_req, 0, sizeof(if_req));
	if_req.ir_interface = res->ai_recv_addrs[0].gsr_interface;
	if_req.ir_scope_id  = 0;
  
  struct pgm_sockaddr_t addr;
	memset (&addr, 0, sizeof(addr));
	addr.sa_port = MULTICAST_PORT;
	addr.sa_addr.sport = DEFAULT_DATA_SOURCE_PORT;
  
	if (!pgm_gsi_create_from_hostname (&addr.sa_addr.gsi, &pgm_err)) {
    std::cerr << "fail creating GSI" << std::endl;
	}
    
  if (!pgm_bind3 (socket_, &addr, sizeof(addr), 
                  &if_req, sizeof(if_req),	/* tx interface */
                  &if_req, sizeof(if_req),	/* rx interface */
                  &pgm_err))
	{
    std::cerr << "fail binding socket: " << pgm_err->message << std::endl;
	}
  
  if (!pgm_connect (socket_, &pgm_err)) 
  {
    std::cerr << "fail connecting socket: " << pgm_err->message << std::endl;
	}
}

Multicast::~Multicast()
{
}

void Multicast::broadcast()
{
  char buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE);
  memcpy(buffer, hostname().c_str(), BUFFER_SIZE);
  
  const int status = pgm_send(socket_, buffer, BUFFER_SIZE, NULL);
  if (PGM_IO_STATUS_NORMAL != status) 
  {
    std::cerr << "failed to send broadcast" << std::endl;
  }
}

std::string Multicast::receive()
{
  char buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE);

  pgm_error_t* pgm_err = NULL;
  const int status = pgm_recv(socket_, buffer, BUFFER_SIZE, 0, 0, &pgm_err);
  
  if (PGM_IO_STATUS_NORMAL != status)
  {
    std::cerr << "fail receiving" << std::endl;
  }
  
  return std::string(buffer);
}