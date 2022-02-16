#include "modem_manager.h"

#include <store/strings.h>

// #define LOG_ON

#ifdef LOG_ON
#define LOGGING(x) LOGGING({ cout << EOL << uint16_t(__LINE__) << ' ' << __func__ << ' '; x
#else
#define LOGGING(x)
#endif



char host[]     = "xxxxxxxxxxxxxxxxxxxxxx";
char port[]     = "80";
char path[]     = "/ocpp/websocket/CentralSystemService/1";
char protocol[] = "ocpp1.6";

namespace gsm {

ManGSM::ManGSM():
    cout(mcu::Serial0::init())
    , m_Debug(true)
    , send_schedule_size(0)
    , on_Recv(NULL)
    , sock_fd(0)
    , m_Connected(false)
    , m_Busy(true)
{
    cout << stream::PGM << PSTR("GSM module ");
    std::cout << "CONNECT...\n";
    std::cout << "Hostname: " << host << ":" << port << "\n";
    
    struct addrinfo hints, *servinfo, *p;
    int rv;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if ((rv = getaddrinfo(host, port, &hints, &servinfo)) != 0)
    {
        //return WS_OPEN_CONNECTION_ADDRINFO_ERR;
    }
    
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sock_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            continue;
        }
        if (::connect(sock_fd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sock_fd);
            continue;
        }
        break;
    }
    freeaddrinfo(servinfo);
    
    struct timeval tv;
    tv.tv_sec  = 0;
    tv.tv_usec = 1000; //500000;
    setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    //-----------------------------------------
    m_Connected = true;
    m_Busy = false;
}


void ManGSM::write(char c) {
    send_buff[m_Writted++] = c;
    if (send_schedule_size == m_Writted) {
        std::cout << "SEND DATA:" << send_buff << '\n';
        ssize_t ddd = send(sock_fd, &send_buff, send_schedule_size, 0);
        std::cout << "SENT:" << ddd << '\n';
    } else if (send_schedule_size < m_Writted) {
        std::cout << "\nSchedule !!! OVERFULL !!!\n";
    }
}


void ManGSM::onEvent(const event::Event& event) {
    
    switch ((events)event.id) {
        case events::UPDATE:
            memset(recv_buff, 0, sizeof(recv_buff));
            // NET
            m_Readed = ::recv(sock_fd, recv_buff, sizeof(recv_buff), 0);
            if ((m_Readed > 0) && (m_Readed <= sizeof(recv_buff)) && (on_Recv != NULL)) {
                std::cout << "recv_count: " << m_Readed << std::endl;
                for (int x = 0; (x < m_Readed) && (x < sizeof(recv_buff)); ++x) {
                    std::cout << char (recv_buff[x]);
                }
                on_Recv(recv_buff, m_Readed);
            }        
            break;
        case events::TICK_1_SEC:
            break;
        case events::GSM_MODE_SET:
            cout << stream::PGM << PSTR("Modem ");
            if (event.param == 0) {
                cout << stream::PGM << PSTR("OFF\r\n");
            } else {
                cout << stream::PGM << PSTR("ON\r\n");
            }
            break;
        case GSM_DEBUG_SET:
            m_Debug = (bool)event.param;
            break;
        default: {}
    }
}

}
