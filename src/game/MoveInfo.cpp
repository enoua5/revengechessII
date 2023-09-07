#include "game/moveinfo.h"
#include "nlohmann/json.hpp"
#include <string>
#include <vector>

std::string MoveInfo::serialize()
{
    nlohmann::json json = {
        { "move", {
            { "from", {
                { "file", move.from.file },
                { "rank", move.from.rank }
            } },
            { "to", {
                { "file", move.to.file },
                { "rank", move.to.rank }
            } },
            { "promotion", move.promotion }
        } },
        { "movedPiece", movedPiece },
        { "wasCapture", wasCapture },
        { "valid", valid },
        { "respawns", respawns }
    };

    return json.dump();
}

MoveInfo::MoveInfo()
{
    respawns = std::vector<std::vector<PieceIdentifier>>();
    valid = false;
    wasCapture = false;
    movedPiece = EMPTY;
    move = Move();

}

MoveInfo::MoveInfo(std::string data)
{
    nlohmann::json parsed = nlohmann::json::parse(data);

    movedPiece = parsed.value("movedPiece", EMPTY);
    wasCapture = parsed.value("wasCapture", false);
    valid = parsed.value("valid", false);
    respawns = std::vector<std::vector<PieceIdentifier>>();
    auto respawns_json = parsed.value("respawns", nlohmann::json::array());
    for(auto &wave_json : respawns_json)
    {
        std::vector<PieceIdentifier> wave = std::vector<PieceIdentifier>();
        for(auto &pi : wave_json)
        {
            wave.push_back(pi);
        }
        respawns.push_back(wave);
    }

    auto fromFile = parsed.at("move").at("from").value("file", -1);
    auto fromRank = parsed.at("move").at("from").value("rank", -1);
    auto toFile = parsed.at("move").at("to").value("file", -1);
    auto toRank = parsed.at("move").at("to").value("rank", -1);
    PieceType promotion = parsed.at("move").value("promotion", NO);

    move = Move(Square(fromFile, fromRank), Square(toFile, toRank), promotion);

}