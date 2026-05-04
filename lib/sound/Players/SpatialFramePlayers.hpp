#pragma once


#include "Player.hpp"
#include "PlayersPool.hpp"

#include <concurrency/TripleBuffer.hpp>

#include <vector>


class SpatialFramePlayers {

public:

    struct PlayerCreateInfo {

        PlayersPool::Handle playerHandle = PlayersPool::Handle::Invalid;
        double posOffset = 0.;
        float  volume    = 0.f;
    };

    class Writer {

    public:

        explicit Writer (SpatialFramePlayers &players);

        ~Writer ();

        void addPlayer (PlayerCreateInfo info) const;

    private:

        SpatialFramePlayers &players_;
        std::vector<PlayerCreateInfo> &writeBuf_;
    };

    class Reader {

    public:

        explicit Reader (SpatialFramePlayers &players);

        void buildPlayers (const PlayersPool::Reader &poolReader);

        std::vector<Player>& getPlayers () const;

    private:

        SpatialFramePlayers &players_;
        bool isNewPlayers_;
    };

public:

    SpatialFramePlayers () = default;

    Writer getWriter ();
    Reader getReader ();

private:

    std::vector<Player> players_;

    TripleBuffer<std::vector<PlayerCreateInfo>> buf_;
};
