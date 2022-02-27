#include "modem_manager.h"
#include "store/strings.h"

namespace gsm {

    ManGSM::ManGSM():
    cout(mcu::Serial0::init())
{
    cout << stream::PGM << PSTR("GSM module ");
}

void ManGSM::start_connect() {
    
    m_Debug = true;
    send_schedule_size = 0;
    on_Recv = NULL;
    sock_fd = 0;
    m_Connected = false;
    m_Busy = true;
    
    
    
    
    
    std::cout << "start_connect...\n";
    std::cout << "Hostname: " << WSC_host << ":" << WSC_port << "\n";
    
    struct addrinfo hints, *servinfo, *p;
    int rv;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if ((rv = getaddrinfo(WSC_host, WSC_port, &hints, &servinfo)) != 0) { exit(1); }
    
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
    std::cout << "connected!\n";
}


void ManGSM::write(char c) {
    tx_buf[m_Writted++] = c;
    if (send_schedule_size == m_Writted) {
        ssize_t ddd = send(sock_fd, tx_buf, send_schedule_size, 0);
    } else if (send_schedule_size < m_Writted) {
        std::cout << "\n Write !!! OVERFULL !!!\n";
    }
}


void ManGSM::onEvent(const event::Event& event) {
    
    switch ((events)event.id) {
        case events::UPDATE:
            memset(rx_buf, 0, rx_sz);
            // NET
            m_Readed = ::recv(sock_fd, rx_buf, rx_sz, 0);
            if ((m_Readed > 0) && (m_Readed <= rx_sz) && (on_Recv != NULL)) {
                
                cout << (char *)rx_buf << EOL;
                on_Recv(rx_buf, m_Readed);
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
            } // TODO GSM_MODE_SET
            break;
        case GSM_DEBUG_SET:
            m_Debug = (bool)event.param;
            break;
        default: {}
    }
}

}
 
