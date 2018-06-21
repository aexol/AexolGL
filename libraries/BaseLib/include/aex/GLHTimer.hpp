#ifndef GLHTIMER_HPP_
#define GLHTIMER_HPP_

#include <aex/Common.hpp>
#include <iostream>

class AEX_PUBLIC_API GLhTimer
{
public:
    GLhTimer();
    ~GLhTimer();

    void start(); // rozpoczyna odliczanie globalnego czasu
    void stop(); // stopuje odliczanie czasu ( ale pamieta ile globalnie odliczylo)
    void restart(); // resetuje zegar, odlicza od 0 sekund znowu globalny czas
    double tick(); // ustawia flage na aktualnym czasie i zwraca czas od ostatniego wywolania funkcji tick (albo od startu)
    double allTime(); // zwraca caly czas naliczony bez uzywania funkcji restart
    bool isWorking(); // czy zlicza czas

private:
    bool m_bIsWorking; // czy mierzy czas

    double m_dStartTime; // zapamietuje czas startu ()
    double m_dStopTime; // czas zatrzymania odliczania(aby wiedzied, ile czasu dodac do StartTime, aby dalej odmierzac czas.
    double m_dLastTickTime; // zapamietuje czas ostatniego wywolania tick (aktualny czas minus ta zmiena ==tick())
    double m_dBuffer; // bufor dla chwilowych wartosci
};

enum GLH_FPS_COUNTER
{
    GLH_FPS_COUNTER_TEMPORARY = 0, // enum oznaczajacy liczenie fps aktualnego. (niedokladne)
    GLH_FPS_COUNTER_SAMPLED, // enum oznaczajacy liczenie fps probkujac co iles sekund (dokladne)
    GLH_FPS_COUNTER_GLOBAL // enum oznaczajacy liczenie fps biorac pod uwage wszystkie wyrenderowane ramki na caly czas (niedokladne)
};

/**
 * Klasa do latwego liczenia ramek na sekunde (3 trybami, bierzaca, co sekunda, globalna (caly czas))
 */
class AEX_PUBLIC_API GLhFPSCounter
{
public:


    /** Konstruktor kreuje obiekt, ustawia czas, co jaki maja byc liczone fps probkowane co jakis czas
     * @param timeOfSample **/
    explicit GLhFPSCounter(bool countWhileFPS = true, bool countSampledFPS = true,
                           bool countGlobalFPS = true, float samplingTime = 1.0f);
    ~GLhFPSCounter();

    void startOfFrame(); // funkcja do na początku głównej pętli gry
    void endOfFrame(); // Funkcja do wywołania na końcu głównej pętli gry

    /** Zwraca aktualna wartosc fps z jednego z 3 sposobow liczenia (aktualny, na sekunde i caly)
     * @param fpsType -wartosc enumeryczna opisujaca sposob liczenia FPS
     * @return fps **/
    float fps(enum GLH_FPS_COUNTER  fpsType);
    bool isNewSampledFPS()
    {
        return m_bNewSampledFPS;
    }

    friend std::ostream& operator << (std::ostream& out, GLhFPSCounter& counter);

private:
    /* Jakie fpsy liczyc */
    bool m_bTmpFPS;
    bool m_bSampledFPS;
    bool m_bGlobalFPS;

    /* Chwilowe fps */
    double m_dTemporaryStartTime; // czas na poczatku glownej petli
    float m_fTemporaryFPS;

    /* Probkowane fps */
    double m_dSampleStartTime;
    float m_fSamplingTime; // czas, przez jaki maja byc zliczane ramki, dla obliczenia FPS
    float m_fSampledFPS; // do przechowywania wartosci wyliczonych fps
    unsigned m_uSampledFrames; // aktualna ilosc ramek dla Trybu sampled
    bool m_bNextSample;

    bool m_bNewSampledFPS;

    /* Globalne fps */
    double m_dGlobalStartTime; // Poczatek odliczania ramek
    long long unsigned m_uGlobalFrames; // Globalna ilosc wygenerowanych ramek
    float m_fGlobalFPS;

    /* Dla wszystkich */
    double m_dBuffer; // czas na koncu glownej petli
};


#endif /* GLHTIMER_HPP_ */
