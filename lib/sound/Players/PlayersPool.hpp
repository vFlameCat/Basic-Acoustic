#pragma once


#include "Player.hpp"

#include <containers/SlotPool.hpp>
#include <concurrency/SPSCQueue.hpp>

#include <variant>


class PlayersPool {

public:

    using Handle = fc::SlotPool<Player>::Handle;

private:

    struct Command {

        struct Add {

            Player player;
        };

        struct Remove {

            Handle handle;
        };

        std::variant<std::monostate, Add, Remove> data;
    };

public:

    class Writer {

    public:

        explicit Writer (PlayersPool &pool);

        Handle addPlayer    (Player player);
        void   removePlayer (Handle handle);

    private:

        PlayersPool &pool_;
    };

    class Reader {

    public:

        explicit Reader (PlayersPool &pool);

        Player::CreateInfo getPlayerCreateInfo (Handle handle) const;

        fc::SlotPool<Player>& getPlayers () const;

    private:

        template<class... Ts>
        struct overloads : Ts... { using Ts::operator()...; };

        void parseCommandAdd    (Command::Add &cmd);
        void parseCommandRemove (Command::Remove &cmd);

    private:

        PlayersPool &pool_;
    };

public:

    PlayersPool () = default;

    Writer getWriter ();
    Reader getReader ();

private:

    // Main thread allocates handles via mainHandles_; the audio thread mirrors
    // the same insert/erase sequence on players_, so handles stay consistent.
    // Both pools share Tag = Player so their Handle types are identical.
    fc::SlotPool<std::monostate, Player> mainHandles_;
    fc::SlotPool<Player>                 players_;

    SPSCQueue<Command, 1024> cmdQueue_;
};
