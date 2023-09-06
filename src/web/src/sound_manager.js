
const SFX = {
    move: [
        new Audio("sound/move.1.mp3"),
        new Audio("sound/move.2.mp3"),
        new Audio("sound/move.3.mp3"),
        new Audio("sound/move.4.mp3"),
        new Audio("sound/move.5.mp3"),
    ],
    capture: [

        new Audio("sound/capture.1.mp3"),
        new Audio("sound/capture.2.mp3"),
        new Audio("sound/capture.3.mp3"),
        new Audio("sound/capture.4.mp3"),
        new Audio("sound/capture.5.mp3"),
    ]
}

function force_play(sound)
{
    // if you call .play() on a currently playing audio track, nothing happens
    // we just need to restart it if it's already going
    if(sound.paused)
        sound.play();
    else
        sound.currentTime = 0;
}

function play_move_sfx()
{
    if(Settings.sound)
    {
        let sound_to_play = randomSelection(SFX.move);
        force_play(sound_to_play)
    }
}

function play_capture_sfx()
{
    if(Settings.sound)
    {
        let sound_to_play = randomSelection(SFX.capture);
        force_play(sound_to_play)
    }
}

