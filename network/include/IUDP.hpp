/*
** EPITECH PROJECT, 2024
** RType
** File description:
** abstract udp server
*/

#ifndef IUDP_HPP
    #define IUDP_HPP

class IUDP {
    public:
        virtual ~IUDP() = default;
    
    private:
        virtual void start_receive() = 0;
};

#endif /* !IUDPSERVER_HPP */
