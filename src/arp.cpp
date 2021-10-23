#include "arp.h"

ARP::ARP(const std::vector<uint8_t>& packet) {
    auto begin_it = packet.begin();
    auto end_it = std::next(begin_it, 2);
    std::move(begin_it, end_it, m_hrd.begin());

    begin_it = end_it;
    end_it = std::next(begin_it, 2);
    std::move(begin_it, end_it, m_pro.begin());

    begin_it = end_it;
    end_it = std::next(begin_it, 1);
    m_hln = *begin_it;

    begin_it = end_it;
    end_it = std::next(begin_it, 1);
    m_pln = *begin_it;

    begin_it = end_it;
    end_it = std::next(begin_it, 2);
    std::move(begin_it, end_it, m_op.begin());

    if (m_hln > 0) {
        m_sha.reserve(m_hln);
        begin_it = end_it;
        end_it = std::next(begin_it, m_hln);
        std::move(begin_it, end_it, std::back_inserter(m_sha));
    }

    if (m_pln > 0) {
        m_spa.reserve(m_pln);
        begin_it = end_it;
        end_it = std::next(begin_it, m_pln);
        std::move(begin_it, end_it, std::back_inserter(m_spa));
    }

    if (m_hln > 0) {
        m_tha.reserve(m_hln);
        begin_it = end_it;
        end_it = std::next(begin_it, m_hln);
        std::move(begin_it, end_it, std::back_inserter(m_tha));
    }

    if (m_pln > 0) {
        m_tpa.reserve(m_pln);
        begin_it = end_it;
        end_it = std::next(begin_it, m_pln);
        std::move(begin_it, end_it, std::back_inserter(m_tpa));
    }
}

auto ARP::get_hrd() const -> std::array<uint8_t, 2> {
    return m_hrd;
}

auto ARP::get_pro() const -> std::array<uint8_t, 2> {
    return m_pro;
}

auto ARP::get_hln() const -> uint8_t {
    return m_hln;
}

auto ARP::get_pln() const -> uint8_t {
    return m_pln;
}

auto ARP::get_op() const -> std::array<uint8_t, 2> {
    return m_op;
}

auto ARP::get_sha() const -> std::vector<uint8_t> {
    return m_sha;
}

auto ARP::get_spa() const -> std::vector<uint8_t> {
    return m_spa;
}

auto ARP::get_tha() const -> std::vector<uint8_t> {
    return m_tha;
}

auto ARP::get_tpa() const -> std::vector<uint8_t> {
    return m_tpa;
}

void ARP::handle() const {
    /*
    ?Do I have the hardware type in ar$hrd?
    Yes: (almost definitely)
        [optionally check the hardware length ar$hln]
        ?Do I speak the protocol in ar$pro?
        Yes:
            [optionally check the protocol length ar$pln]
            Merge_flag := false
            If the pair <protocol type, sender protocol address> is
            already in my translation table, update the sender
            hardware address field of the entry with the new
            information in the packet and set Merge_flag to true.
            ?Am I the target protocol address?
            Yes:
                If Merge_flag is false, add the triplet <protocol type,
                sender protocol address, sender hardware address> to
                the translation table.
                ?Is the opcode ares_op$REQUEST?  (NOW look at the opcode!!)
                Yes:
                    Swap hardware and protocol fields, putting the local
                    hardware and protocol addresses in the sender fields.
                    Set the ar$op field to ares_op$REPLY
                    Send the packet to the (new) target hardware address on
                    the same hardware on which the request was received.
     */
}
