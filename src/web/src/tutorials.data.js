var tutorials = {
    "winning": {
        "title": "Winning",
        "description": "In Revenge Chess, games are won not by checkmate, but by leaving the opponent without a king.\nThe details of why this is, and the strategies it enables are left for later tutorials.\n\nIn this tutorial, you are in a sitation that would normally be mate-in-one in regular chess.\nHowever, due to need to capture the enemy king, it is actually mate-in-two.",
        "board_state": {
            "turn": true,
            "pawnEnPassantFile": "X",
            "halfMoveClock": 0,
            "pieces": [
                {"isOnBoard": true, "hasMoved": true, "color": true, "type": "ROOK", "home": "A1", "current": "A4", "captures": ["BLACK_KINGSIDE_BISHOP","BLACK_KINGSIDE_KNIGHT"]},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "KNIGHT", "home": "B1", "current": "B1", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "BISHOP", "home": "C1", "current": "C1", "captures": []},
                {"isOnBoard": false, "hasMoved": true, "color": true, "type": "QUEEN", "home": "D1", "current": "H5", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": true, "type": "ROOK", "home": "H1", "current": "E2", "captures": ["BLACK_QUEENSIDE_ROOK"]},
                {"isOnBoard": true, "hasMoved": true, "color": true, "type": "KNIGHT", "home": "G1", "current": "F5", "captures": ["BLACK_E_PAWN", "BLACK_D_PAWN","BLACK_KINGSIDE_ROOK"]},
                {"isOnBoard": true, "hasMoved": true, "color": true, "type": "BISHOP", "home": "F1", "current": "G4", "captures": ["BLACK_A_PAWN","BLACK_B_PAWN"]},
                {"isOnBoard": true, "hasMoved": true, "color": true, "type": "KING", "home": "E1", "current": "G1", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": true, "type": "PAWN", "home": "A2", "current": "A5", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": true, "type": "PAWN", "home": "B2", "current": "B4", "captures": []},
                {"isOnBoard": false, "hasMoved": true, "color": true, "type": "PAWN", "home": "C2", "current": "C4", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "PAWN", "home": "D2", "current": "D2", "captures": []},
                {"isOnBoard": false, "hasMoved": true, "color": true, "type": "PAWN", "home": "E2", "current": "E4", "captures": []},
                {"isOnBoard": true, "hasMoved": false, "color": true, "type": "PAWN", "home": "F2", "current": "F2", "captures": []},
                {"isOnBoard": true, "hasMoved": false, "color": true, "type": "PAWN", "home": "G2", "current": "G2", "captures": []},
                {"isOnBoard": true, "hasMoved": false, "color": true, "type": "PAWN", "home": "H2", "current": "H2", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "ROOK", "home": "A8", "current": "D8", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": false, "type": "KNIGHT", "home": "B8", "current": "D5", "captures": ["WHITE_QUEEN","WHITE_QUEENSIDE_BISHOP"]},
                {"isOnBoard": true, "hasMoved": false, "color": false, "type": "BISHOP", "home": "C8", "current": "C8", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": false, "type": "QUEEN", "home": "D8", "current": "C3", "captures": ["WHITE_QUEENSIDE_KNIGHT","WHITE_C_PAWN","WHITE_D_PAWN","WHITE_E_PAWN"]},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "ROOK", "home": "H8", "current": "H8", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "KNIGHT", "home": "G8", "current": "G8", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "BISHOP", "home": "F8", "current": "F8", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": false, "type": "KING", "home": "E8", "current": "G8", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "PAWN", "home": "A7", "current": "A7", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "PAWN", "home": "B7", "current": "B7", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": false, "type": "PAWN", "home": "C7", "current": "C4", "captures": []},
                {"isOnBoard": false, "hasMoved": true, "color": false, "type": "PAWN", "home": "D7", "current": "D5", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "PAWN", "home": "E7", "current": "E7", "captures": []},
                {"isOnBoard": true, "hasMoved": false, "color": false, "type": "PAWN", "home": "F7", "current": "F7", "captures": []},
                {"isOnBoard": true, "hasMoved": false, "color": false, "type": "PAWN", "home": "G7", "current": "G7", "captures": []},
                {"isOnBoard": true, "hasMoved": false, "color": false, "type": "PAWN", "home": "H7", "current": "H7", "captures": []}
            ]
        },
        "ai_settings": {"usingAI":true,"limitMode":2,"maxTime":10000,"limitSeconds":15000,"limitPlys":3,"flagBuffer":500,"minDelta":0,"maxDelta":0}
    },


    "respawning": {
        "title": "Respawning",
        "description": "The main new mechanic in Revenge Chess is the abilty to respawn pieces that had previously been captured.\nTo do this, simply capture back the enemy piece that captured the piece you lost.\n\nIn this tutorial, the position would normally be lost for white, but through respawning, it can be won!\nNote: The position in this tutorial has been simplified, and is not actually reachable in normal play",
        "board_state": {
            "turn": true,
            "pawnEnPassantFile": "X",
            "halfMoveClock": 0,
            "pieces": [
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "ROOK", "home": "A1", "current": "A1", "captures": []},
                {"isOnBoard": true, "hasMoved": false, "color": true, "type": "KNIGHT", "home": "B1", "current": "A7", "captures": []},
                {"isOnBoard": true, "hasMoved": false, "color": true, "type": "BISHOP", "home": "C1", "current": "C8", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "QUEEN", "home": "D1", "current": "D1", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "ROOK", "home": "H1", "current": "H1", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "KNIGHT", "home": "G1", "current": "G1", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "BISHOP", "home": "F1", "current": "F1", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": true, "type": "KING", "home": "E1", "current": "G1", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "PAWN", "home": "A2", "current": "A2", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "PAWN", "home": "B2", "current": "B2", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": true, "type": "PAWN", "home": "C2", "current": "C6", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "PAWN", "home": "D2", "current": "D2", "captures": []},
                {"isOnBoard": true, "hasMoved": false, "color": true, "type": "PAWN", "home": "E2", "current": "E5", "captures": []},
                {"isOnBoard": true, "hasMoved": false, "color": true, "type": "PAWN", "home": "F2", "current": "F2", "captures": []},
                {"isOnBoard": true, "hasMoved": false, "color": true, "type": "PAWN", "home": "G2", "current": "G2", "captures": []},
                {"isOnBoard": true, "hasMoved": false, "color": true, "type": "PAWN", "home": "H2", "current": "H2", "captures": []},
                {"isOnBoard": false, "hasMoved": true, "color": false, "type": "ROOK", "home": "A8", "current": "B2", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": false, "type": "KNIGHT", "home": "B8", "current": "G4", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "BISHOP", "home": "C8", "current": "C8", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": false, "type": "QUEEN", "home": "D8", "current": "A2", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": false, "type": "ROOK", "home": "H8", "current": "E8", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": false, "type": "KNIGHT", "home": "G8", "current": "E3", "captures": ["WHITE_QUEEN"]},
                {"isOnBoard": true, "hasMoved": true, "color": false, "type": "BISHOP", "home": "F8", "current": "F4", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": false, "type": "KING", "home": "E8", "current": "D8", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "PAWN", "home": "A7", "current": "A7", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "PAWN", "home": "B7", "current": "B7", "captures": []},
                {"isOnBoard": true, "hasMoved": false, "color": false, "type": "PAWN", "home": "C7", "current": "C7", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "PAWN", "home": "D7", "current": "D7", "captures": []},
                {"isOnBoard": true, "hasMoved": false, "color": false, "type": "PAWN", "home": "E7", "current": "E7", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "PAWN", "home": "F7", "current": "F7", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "PAWN", "home": "G7", "current": "G7", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "PAWN", "home": "H7", "current": "H7", "captures": []}
            ]
        },
        "ai_settings": {"usingAI":true,"limitMode":2,"maxTime":10000,"limitSeconds":15000,"limitPlys":3,"flagBuffer":500,"minDelta":0,"maxDelta":0}
    },


    "respawn-cap": {
        "title": "Capture by Respawn",
        "description": "When respawning, pieces always return to their starting square.\nIf that square happens to be occupied, the respawning piece will capture the piece that's in the way.\n\nHere is another position that would normally be lost for white.\nHowever, the black king is standing on a captured white piece's square...\n\nNote: like the last tutorial, this position is simplified, and not actually reachable in a real game.",
        "board_state": {
            "turn": true,
            "pawnEnPassantFile": "X",
            "halfMoveClock": 0,
            "pieces": [
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "ROOK", "home": "A1", "current": "A1", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "KNIGHT", "home": "B1", "current": "B1", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "BISHOP", "home": "C1", "current": "C1", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "QUEEN", "home": "D1", "current": "D1", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "ROOK", "home": "H1", "current": "H1", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "KNIGHT", "home": "G1", "current": "G1", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "BISHOP", "home": "F1", "current": "F1", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": true, "type": "KING", "home": "E1", "current": "G1", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "PAWN", "home": "A2", "current": "A2", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "PAWN", "home": "B2", "current": "B2", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "PAWN", "home": "C2", "current": "C2", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "PAWN", "home": "D2", "current": "D2", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": true, "type": "PAWN", "home": "E2", "current": "E6", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "PAWN", "home": "F2", "current": "F2", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "PAWN", "home": "G2", "current": "G2", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "PAWN", "home": "H2", "current": "H2", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": false, "type": "ROOK", "home": "A8", "current": "D7", "captures": ["WHITE_A_PAWN"]},
                {"isOnBoard": true, "hasMoved": true, "color": false, "type": "KNIGHT", "home": "B8", "current": "H4", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "BISHOP", "home": "C8", "current": "C8", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": false, "type": "QUEEN", "home": "D8", "current": "G2", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "ROOK", "home": "H8", "current": "H8", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "KNIGHT", "home": "G8", "current": "G8", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "BISHOP", "home": "F8", "current": "F8", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": false, "type": "KING", "home": "E8", "current": "A2", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "PAWN", "home": "A7", "current": "A7", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "PAWN", "home": "B7", "current": "B7", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "PAWN", "home": "C7", "current": "C7", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "PAWN", "home": "D7", "current": "D7", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "PAWN", "home": "E7", "current": "E7", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "PAWN", "home": "F7", "current": "F7", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "PAWN", "home": "G7", "current": "G7", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "PAWN", "home": "H7", "current": "H7", "captures": []}
            ]
        },
        "ai_settings": {"usingAI":true,"limitMode":2,"maxTime":10000,"limitSeconds":15000,"limitPlys":3,"flagBuffer":500,"minDelta":0,"maxDelta":0}
    },


    "king-respawn": {
        "title": "King Repawn",
        "description": "The game is won or lost when a turn ends with one of the players missing their king.\nHowever, that doesn't mean the king can't get captured during the turn!\nIf a king is captured and respawned in a single move, play continues...\n\nNote: once again, the board has been simplified, and the position is not actually reachable in normal play.",
        "board_state": {
            "turn": true,
            "pawnEnPassantFile": "X",
            "halfMoveClock": 0,
            "pieces": [
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "ROOK", "home": "A1", "current": "A1", "captures": []},
                {"isOnBoard": false, "hasMoved": true, "color": true, "type": "KNIGHT", "home": "B1", "current": "D5", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": true, "type": "BISHOP", "home": "C1", "current": "H2", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "QUEEN", "home": "D1", "current": "D1", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "ROOK", "home": "H1", "current": "H1", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "KNIGHT", "home": "G1", "current": "G1", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "BISHOP", "home": "F1", "current": "F1", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": true, "type": "KING", "home": "E1", "current": "B7", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "PAWN", "home": "A2", "current": "A2", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "PAWN", "home": "B2", "current": "B2", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "PAWN", "home": "C2", "current": "C2", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "PAWN", "home": "D2", "current": "D2", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "PAWN", "home": "E2", "current": "E2", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "PAWN", "home": "F2", "current": "F2", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "PAWN", "home": "G2", "current": "G2", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "PAWN", "home": "H2", "current": "H2", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": false, "type": "ROOK", "home": "A8", "current": "A5", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": false, "type": "KNIGHT", "home": "B8", "current": "A4", "captures": []},
                {"isOnBoard": true, "hasMoved": false, "color": false, "type": "BISHOP", "home": "C8", "current": "C8", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": false, "type": "QUEEN", "home": "D8", "current": "A8", "captures": ["WHITE_QUEEN"]},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "ROOK", "home": "H8", "current": "H8", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": false, "type": "KNIGHT", "home": "G8", "current": "E8", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "BISHOP", "home": "F8", "current": "F8", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": false, "type": "KING", "home": "E8", "current": "D7", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "PAWN", "home": "A7", "current": "A7", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": false, "type": "PAWN", "home": "B7", "current": "B6", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": false, "type": "PAWN", "home": "C7", "current": "C6", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": false, "type": "PAWN", "home": "D7", "current": "E6", "captures": []},
                {"isOnBoard": true, "hasMoved": false, "color": false, "type": "PAWN", "home": "E7", "current": "E7", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "PAWN", "home": "F7", "current": "F7", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "PAWN", "home": "G7", "current": "G7", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "PAWN", "home": "H7", "current": "H7", "captures": []}
            ]
        },
        "ai_settings": {
            "usingAI":true,
            "limitMode":2,
            "maxTime":10000,
            "limitSeconds":15000,
            "limitPlys":3,
            "flagBuffer":500,
            "minDelta":0,
            "maxDelta":0,
            "book": {
                "B7C8": {
                    "A8C8": {}
                }
            }
        }
    },



    "king-promo": {
        "title": "Promotion to King",
        "description": "Since Revenge Chess doesn't use check, we decided it would be interesting to allow promotion to a king.\nThe game is only lost if you have no kings left on the board,\nso making a second king can get you out of a tricky spot!\n\nNote: once again, this is a simplified position",
        "board_state": {
            "turn": true,
            "pawnEnPassantFile": "X",
            "halfMoveClock": 0,
            "pieces": [
                {"isOnBoard": true, "hasMoved": true, "color": true, "type": "ROOK", "home": "A1", "current": "G2", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "KNIGHT", "home": "B1", "current": "B1", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "BISHOP", "home": "C1", "current": "C1", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": true, "type": "QUEEN", "home": "D1", "current": "F7", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "ROOK", "home": "H1", "current": "F7", "captures": []},
                {"isOnBoard": false, "hasMoved": true, "color": true, "type": "KNIGHT", "home": "G1", "current": "G7", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "BISHOP", "home": "F1", "current": "F1", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": true, "type": "KING", "home": "E1", "current": "A1", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "PAWN", "home": "A2", "current": "A2", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "PAWN", "home": "B2", "current": "B2", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "PAWN", "home": "C2", "current": "C2", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "PAWN", "home": "D2", "current": "D7", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": true, "type": "PAWN", "home": "E2", "current": "E7", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "PAWN", "home": "F2", "current": "F6", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "PAWN", "home": "G2", "current": "G2", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": true, "type": "PAWN", "home": "H2", "current": "H2", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": false, "type": "ROOK", "home": "A8", "current": "B1", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": false, "type": "KNIGHT", "home": "B8", "current": "C3", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "BISHOP", "home": "C8", "current": "C8", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "QUEEN", "home": "D8", "current": "B5", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "ROOK", "home": "H8", "current": "H8", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "KNIGHT", "home": "G8", "current": "G8", "captures": []},
                {"isOnBoard": false, "hasMoved": true, "color": false, "type": "BISHOP", "home": "F8", "current": "F8", "captures": []},
                {"isOnBoard": true, "hasMoved": true, "color": false, "type": "KING", "home": "E8", "current": "H8", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "PAWN", "home": "A7", "current": "A7", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "PAWN", "home": "B7", "current": "B7", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "PAWN", "home": "C7", "current": "C7", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "PAWN", "home": "D7", "current": "D7", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "PAWN", "home": "E7", "current": "E7", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "PAWN", "home": "F7", "current": "F7", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "PAWN", "home": "G7", "current": "G7", "captures": []},
                {"isOnBoard": false, "hasMoved": false, "color": false, "type": "PAWN", "home": "H7", "current": "H7", "captures": []}
            ]
        },
        "ai_settings": {"usingAI":true,"limitMode":2,"maxTime":10000,"limitSeconds":15000,"limitPlys":3,"flagBuffer":500,"minDelta":0,"maxDelta":0}
    }
};

/*
"": {
    "title": "",
    "description": "",
    "board_state": {
        "turn": true,
        "pawnEnPassantFile": "X",
        "halfMoveClock": 0,
        "pieces": [
            {"isOnBoard": true, "hasMoved": false, "color": true, "type": "ROOK", "home": "A1", "current": "A1", "captures": []},
            {"isOnBoard": true, "hasMoved": false, "color": true, "type": "KNIGHT", "home": "B1", "current": "B1", "captures": []},
            {"isOnBoard": true, "hasMoved": false, "color": true, "type": "BISHOP", "home": "C1", "current": "C1", "captures": []},
            {"isOnBoard": true, "hasMoved": false, "color": true, "type": "QUEEN", "home": "D1", "current": "D1", "captures": []},
            {"isOnBoard": true, "hasMoved": false, "color": true, "type": "ROOK", "home": "H1", "current": "H1", "captures": []},
            {"isOnBoard": true, "hasMoved": false, "color": true, "type": "KNIGHT", "home": "G1", "current": "G1", "captures": []},
            {"isOnBoard": true, "hasMoved": false, "color": true, "type": "BISHOP", "home": "F1", "current": "F1", "captures": []},
            {"isOnBoard": true, "hasMoved": false, "color": true, "type": "KING", "home": "E1", "current": "E1", "captures": []},
            {"isOnBoard": true, "hasMoved": false, "color": true, "type": "PAWN", "home": "A2", "current": "A2", "captures": []},
            {"isOnBoard": true, "hasMoved": false, "color": true, "type": "PAWN", "home": "B2", "current": "B2", "captures": []},
            {"isOnBoard": true, "hasMoved": false, "color": true, "type": "PAWN", "home": "C2", "current": "C2", "captures": []},
            {"isOnBoard": true, "hasMoved": false, "color": true, "type": "PAWN", "home": "D2", "current": "D2", "captures": []},
            {"isOnBoard": true, "hasMoved": false, "color": true, "type": "PAWN", "home": "E2", "current": "E2", "captures": []},
            {"isOnBoard": true, "hasMoved": false, "color": true, "type": "PAWN", "home": "F2", "current": "F2", "captures": []},
            {"isOnBoard": true, "hasMoved": false, "color": true, "type": "PAWN", "home": "G2", "current": "G2", "captures": []},
            {"isOnBoard": true, "hasMoved": false, "color": true, "type": "PAWN", "home": "H2", "current": "H2", "captures": []},
            {"isOnBoard": true, "hasMoved": false, "color": false, "type": "ROOK", "home": "A8", "current": "A8", "captures": []},
            {"isOnBoard": true, "hasMoved": false, "color": false, "type": "KNIGHT", "home": "B8", "current": "B8", "captures": []},
            {"isOnBoard": true, "hasMoved": false, "color": false, "type": "BISHOP", "home": "C8", "current": "C8", "captures": []},
            {"isOnBoard": true, "hasMoved": false, "color": false, "type": "QUEEN", "home": "D8", "current": "D8", "captures": []},
            {"isOnBoard": true, "hasMoved": false, "color": false, "type": "ROOK", "home": "H8", "current": "H8", "captures": []},
            {"isOnBoard": true, "hasMoved": false, "color": false, "type": "KNIGHT", "home": "G8", "current": "G8", "captures": []},
            {"isOnBoard": true, "hasMoved": false, "color": false, "type": "BISHOP", "home": "F8", "current": "F8", "captures": []},
            {"isOnBoard": true, "hasMoved": false, "color": false, "type": "KING", "home": "E8", "current": "E8", "captures": []},
            {"isOnBoard": true, "hasMoved": false, "color": false, "type": "PAWN", "home": "A7", "current": "A7", "captures": []},
            {"isOnBoard": true, "hasMoved": false, "color": false, "type": "PAWN", "home": "B7", "current": "B7", "captures": []},
            {"isOnBoard": true, "hasMoved": false, "color": false, "type": "PAWN", "home": "C7", "current": "C7", "captures": []},
            {"isOnBoard": true, "hasMoved": false, "color": false, "type": "PAWN", "home": "D7", "current": "D7", "captures": []},
            {"isOnBoard": true, "hasMoved": false, "color": false, "type": "PAWN", "home": "E7", "current": "E7", "captures": []},
            {"isOnBoard": true, "hasMoved": false, "color": false, "type": "PAWN", "home": "F7", "current": "F7", "captures": []},
            {"isOnBoard": true, "hasMoved": false, "color": false, "type": "PAWN", "home": "G7", "current": "G7", "captures": []},
            {"isOnBoard": true, "hasMoved": false, "color": false, "type": "PAWN", "home": "H7", "current": "H7", "captures": []}
        ]
    },
    "ai_settings": {"usingAI":true,"limitMode":2,"maxTime":10000,"limitSeconds":15000,"limitPlys":3,"flagBuffer":500,"minDelta":0,"maxDelta":0}
}
*/