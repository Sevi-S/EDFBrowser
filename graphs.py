import numpy as np
import matplotlib.pyplot as plt
import os
import pyedflib as edf
import sys

from scipy.signal import find_peaks_cwt, welch
from scipy import stats
from scipy.signal import butter, lfilter, freqz

from mne.filter import filter_data
from scipy.signal import hilbert, chirp

#path = "C:/Users/Margot Heijmans/Documents/TREMORREG/"
#path = "Y:/Tremor/"

#stuff that goes into .pro file but cant be commented out
#python.path = $${DESTDIR} or = CONTENTS/PACKAGES
#python.files = cmake-build-debug/graphs.py
#INSTALLS or QMAKE_BUNDLE_WHATEVER = python
#https://www.qtcentre.org/threads/48654-QMake-doesnt-do-simple-things-in-Windows

#/Users/Sevi_Pro/Desktop/seviEDFbrow/edfbrowser_170_source/cmake-build-debug/graphs.py
#
#file = ("/Users/Sevi_Pro/Desktop/seviEDFbrow/edfbrowser_170_source/cmake-build-debug/testDoc1.txt")


if(len(sys.argv[1])<15):
    here = os.path.dirname(os.path.abspath(__file__))
    file = os.path.join(here,sys.argv[1])
else:
    print("all args= ", str(sys.argv))
   # here = os.path.dirname(os.path.abspath(__file__))
    file = sys.argv[1]
 #   file = os.path.join(here,sys.argv[1])
#if samp rate is 256 do things other wise do other things

#file = ('/Users/Sevi_Pro/Desktop/wintestfin/EDFbrowser-master/cmake-build-debug/save45')

dataG = []
dataE = []
patientinfo = []
paths = []
offset = []
numTremors = 0
with open(file) as fd:
    for line in fd:
        if line.startswith('%'):
            line = line.strip('%')
            paths.append(list(map(str, line.strip().split(','))))
            continue
        elif line.startswith('&'):
            line = line.strip('&')
            patientinfo.append(list(map(str, line.strip().split(','))))
            continue
        elif line.startswith('@'):
            numTremors = numTremors + 1
            continue    #dothings
        elif line.startswith('$'):
            line = line.strip('$')
            dataG.append(list(map(float, line.strip().split(','))))
            continue
        elif line.startswith('!'):
            line = line.strip('!')
            dataE.append(list(map(float, line.strip().split(','))))
            continue
        elif line.startswith('?'):
            line = line.strip('?')
            offset.append(list(map(float, line.strip().split(','))))
            continue

if offset != []:
    newoffset = [float(d) for d in offset[0]]
else:
    newoffset = [0]

def listtostring(s):
    str1 = ""  #initialize an emptystring
    # traverse in the string
    for ele in s:
        str1 += ele

        # return string
    return str1
#https://www.geeksforgeeks.org/python-program-to-convert-a-list-to-string/


holdstrings = [] #need to append to the end of the string array insted of just add in
i = 0
while i < len(paths):
    holdstrings = holdstrings + paths[i]
    i=i+1

holdstring = listtostring(holdstrings)

count = 0


def signaltonoise(a, axis=0, ddof=0):
    a = np.asanyarray(a)
    m = a.mean(axis)
    sd = a.std(axis=axis, ddof=ddof)
    return np.where(sd == 0, 0, m/sd)
#from scipy, current version no longer includes this
#https://stackoverflow.com/questions/51413068/calculate-signal-to-noise-ratio-in-python-scipy-version-1-1


def SNR(n,s):
    return n/s


def moving_average(a, n=1): #need to covert samples to 100ms window
    ret = np.cumsum(a, dtype=float)
    ret[n:] = ret[n:] - ret[:-n]
    return ret[n - 1:] / n
#https://stackoverflow.com/questions/14313510/how-to-calculate-moving-average-using-numpy


def getsignaltonoise(path):

    val = path.readSignal(2)
    val2 = path.readSignal(2)
    sigNoise1 = signaltonoise(val)
    sigNoise2 = signaltonoise(val2)

    return sigNoise1,sigNoise2


def butter_lowpass(cutoff, fs, order=5):
    nyq = 0.5 * fs
    normal_cutoff = cutoff / nyq
    b, a = butter(order, normal_cutoff, btype='low', analog=False)
    return b, a


def butter_lowpass_filter(data, cutoff, fs, order=5):
    b, a = butter_lowpass(cutoff, fs, order=order)
    y = lfilter(b, a, data)
    return y


def graphtremor(startT,endTim,pathFile,sampRate, count, offs):

    offs = round(offs[0])

    if offset != 0:
        if count == 1:
            startT = startT + offs
            endTim = endTim + offs


    startSamp = sampRate*startT
    endSamp = sampRate*endTim

    startSamp = round(startSamp)
    endSamp = round(endSamp)

    gyrxa = pathFile.readSignal(1)  # this is just here make files have equal lenght(the few lines after this)

    if len(np.arange(startT, endTim, 1/sampRate)) != len(gyrxa[startSamp:endSamp]):
        if len(np.arange(startT, endTim, 1/sampRate)) > len(gyrxa[startSamp:endSamp]):
            endTim = endTim - (1/sampRate)
        else:
            endTim = endTim + (1/sampRate)


    sensor = []

    winsize = 0
    if sampRate == 200:



        sensor = [1.1, 2.2, 3.3, 4.4, 5.5, 6.6]
        accxa = pathFile.readSignal(0)
        accya = pathFile.readSignal(1)
        accza = pathFile.readSignal(2)
        gyrxa = pathFile.readSignal(3)
        gyrya = pathFile.readSignal(4)
        gyrza = pathFile.readSignal(5)
        sensor[3] = accxa[startSamp:endSamp]
        sensor[4] = accya[startSamp:endSamp]
        sensor[5] = accza[startSamp:endSamp]
        sensor[0] = gyrxa[startSamp:endSamp]
        sensor[1] = gyrya[startSamp:endSamp]
        sensor[2] = gyrza[startSamp:endSamp]

        # plotting starts here
        plt.figure(1, figsize=(25, 20))
        plt.suptitle('Wearables #4')
        # plt.suptitle('Activiteit: ' + str(activities.loc[row, 'activity']) + ' - Start video: ' + str(
        # datetime.timedelta(seconds=activities.loc[row, 'start(s)'])), y=1, fontsize=20)
        # set up subplot grid
        # gridspec.GridSpec(4, 6) #lol
        # large subplot raw gyroscope signal
        plt.subplot2grid((4, 6), (0, 0), colspan=2, rowspan=2)
        plt.title('Gyr signal A')
        plt.xlabel('Time(s)')
        plt.ylabel('Amplitude(deg/s)')
        plt.plot(np.arange(startT, endTim, 1 / sampRate), gyrxa[startSamp:endSamp], alpha=0.8)
        plt.plot(np.arange(startT, endTim, 1 / sampRate), gyrya[startSamp:endSamp], alpha=0.6)
        plt.plot(np.arange(startT, endTim, 1 / sampRate), gyrza[startSamp:endSamp], alpha=0.4)
        plt.legend(['x', 'y', 'z'], loc=4)
        # large subplot raw accelerometer signal
        plt.subplot2grid((4, 6), (0, 2), colspan=2, rowspan=2)
        plt.title('Acc signal')
        plt.xlabel('Time(s)')
        plt.ylabel('Amplitude(g)')
        plt.plot(np.arange(startT, endTim, 1 / sampRate), accxa[startSamp:endSamp], alpha=0.6)
        plt.plot(np.arange(startT, endTim, 1 / sampRate), accya[startSamp:endSamp], alpha=0.6)
        plt.plot(np.arange(startT, endTim, 1 / sampRate), accza[startSamp:endSamp], alpha=0.6)
        plt.legend(['x', 'y', 'z'], loc=4)

        f, GYRX = welch(gyrxa, sampRate, nperseg=sampRate)
        f, GYRY = welch(gyrya, sampRate, nperseg=sampRate)
        f, GYRZ = welch(gyrza, sampRate, nperseg=sampRate)
        f, ACCX = welch(accxa, sampRate, nperseg=sampRate)
        f, ACCY = welch(accya, sampRate, nperseg=sampRate)
        f, ACCZ = welch(accza, sampRate, nperseg=sampRate)

        # Middle subplot frequency peak gyroscope
        plt.subplot2grid((4, 6), (0, 4), colspan=2, rowspan=1)
        plt.title('Gyr freq spectrum')
        plt.xlabel('Frequency(Hz)')
        plt.ylabel('Power')
        plt.plot(f, GYRX, f, GYRY, f, GYRZ)
        plt.legend(['x', 'y', 'z'], loc=4)
        plt.xlim(0, 20)

        for i in range(1, 30):
            plt.axvline(x=1 * i, linewidth=0.2, color='grey')
        # Middle subplot frequency peak accelerometer
        plt.subplot2grid((4, 6), (1, 4), colspan=2, rowspan=1)
        plt.title('Acc freq spectrum')
        plt.xlabel('Frequency(Hz)')
        plt.ylabel('Power')
        plt.plot(f, ACCX, f, ACCY, f, ACCZ)
        plt.legend(['x', 'y', 'z'], loc=4)
        plt.xlim(0, 20)

        for i in range(1, 30):
            plt.axvline(x=1 * i, linewidth=0.2, color='grey')

    elif sampRate == 256:
        winsize = round(0.1*sampRate)#100m


        sensor = [1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8]
        ch1 = pathFile.readSignal(0)
        ch2 = pathFile.readSignal(1)
        ch3 = pathFile.readSignal(2)
        ch4 = pathFile.readSignal(3)#correct vals for useful channels
        ch5 = pathFile.readSignal(4)
        ch6 = pathFile.readSignal(5)
        ch7 = pathFile.readSignal(6)
        ch8 = pathFile.readSignal(7)
        sensor[0] = moving_average(np.square(ch1[startSamp:endSamp]),n=winsize)#11008,10983
        sensor[1] = moving_average(np.square(ch2[startSamp:endSamp]),n=winsize)
        sensor[2] = moving_average(np.square(ch3[startSamp:endSamp]),n=winsize)
        sensor[3] = moving_average(np.square(ch4[startSamp:endSamp]),n=winsize)
        sensor[4] = moving_average(np.square(ch5[startSamp:endSamp]),n=winsize)
        sensor[5] = moving_average(np.square(ch6[startSamp:endSamp]),n=winsize)
        sensor[6] = moving_average(np.square(ch7[startSamp:endSamp]),n=winsize)
        sensor[7] = moving_average(np.square(ch8[startSamp:endSamp]),n=winsize)


        sensor[0] = butter_lowpass_filter(sensor[0], cutoff=50, fs=256, order=6)
        sensor[1] = butter_lowpass_filter(sensor[1], cutoff=50, fs=256, order=6)
        sensor[2] = butter_lowpass_filter(sensor[2], cutoff=50, fs=256, order=6)
        sensor[3] = butter_lowpass_filter(sensor[3], cutoff=50, fs=256, order=6)
        sensor[4] = butter_lowpass_filter(sensor[4], cutoff=50, fs=256, order=6)
        sensor[5] = butter_lowpass_filter(sensor[5], cutoff=50, fs=256, order=6)
        sensor[6] = butter_lowpass_filter(sensor[6], cutoff=50, fs=256, order=6)
        sensor[7] = butter_lowpass_filter(sensor[7], cutoff=50, fs=256, order=6)

        #low pass filtering dont do much

        # plotting starts here
        plt.figure(1, figsize=(25, 20))
        plt.suptitle('EMG Graph #4')

        plt.subplot2grid((4, 8), (0, 0), colspan=3, rowspan=2)
        plt.title('Channels 1-4 EMG')
        plt.xlabel('Time(s)')
        plt.ylabel('Amplitude')
        plt.plot(np.arange(startT, endTim-(winsize*(1 / sampRate))+(1 / sampRate), 1 / sampRate), sensor[0], alpha=0.8)#0.09765
        plt.plot(np.arange(startT, endTim-(winsize*(1 / sampRate))+(1 / sampRate), 1 / sampRate), sensor[1], alpha=0.6)
        plt.plot(np.arange(startT, endTim-(winsize*(1 / sampRate))+(1 / sampRate), 1 / sampRate), sensor[2], alpha=0.4)#could loop all of this
        plt.plot(np.arange(startT, endTim-(winsize*(1 / sampRate))+(1 / sampRate), 1 / sampRate), sensor[3], alpha=0.2)
        plt.legend(['Ch1', 'Ch2', 'Ch3','Ch4'], loc=4)
        # large subplot raw accelerometer signal
        plt.subplot2grid((4, 8), (0, 3), colspan=3, rowspan=2)
        plt.title('Channels 5-8 EMG')
        plt.xlabel('Time(s)')
        plt.ylabel('Amplitude')
        plt.plot(np.arange(startT, endTim-(winsize*(1 / sampRate))+(1 / sampRate), 1 / sampRate), sensor[4], alpha=0.9)
        plt.plot(np.arange(startT, endTim-(winsize*(1 / sampRate))+(1 / sampRate), 1 / sampRate), sensor[5], alpha=0.7)
        plt.plot(np.arange(startT, endTim-(winsize*(1 / sampRate))+(1 / sampRate), 1 / sampRate), sensor[6], alpha=0.5)
        plt.plot(np.arange(startT, endTim-(winsize*(1 / sampRate))+(1 / sampRate), 1 / sampRate), sensor[7], alpha=0.3)
        plt.legend(['Ch5', 'Ch6', 'Ch7', 'Ch8'], loc=4)

        f, CH1 = welch(sensor[0], sampRate, nperseg=sampRate)
        f, CH2 = welch(sensor[1], sampRate, nperseg=sampRate)
        f, CH3 = welch(sensor[2], sampRate, nperseg=sampRate)
        f, CH4 = welch(sensor[3], sampRate, nperseg=sampRate)
        f, CH5 = welch(sensor[4], sampRate, nperseg=sampRate)
        f, CH6 = welch(sensor[5], sampRate, nperseg=sampRate)
        f, CH7 = welch(sensor[6], sampRate, nperseg=sampRate)
        f, CH8 = welch(sensor[7], sampRate, nperseg=sampRate)

        # Middle subplot frequency peak gyroscope
        plt.subplot2grid((4, 8), (0, 6), colspan=2, rowspan=1)
        plt.title('Channels 1-4 Frequency Spectrum')
        plt.xlabel('Frequency(Hz)')
        plt.ylabel('Power')
        plt.plot(f, CH1, alpha=0.8)
        plt.plot(f, CH2, alpha=0.6)
        plt.plot(f, CH3, alpha=0.4)
        plt.plot(f, CH4, alpha=0.2)
        plt.legend(['Ch1', 'Ch2', 'Ch3','Ch4'], loc=4)
        plt.xlim(0, 20)

        for i in range(1, 30):
            plt.axvline(x=1 * i, linewidth=0.2, color='grey')

        # Middle subplot frequency peak accelerometer
        plt.subplot2grid((4, 8), (1, 6), colspan=2, rowspan=1)
        plt.title('Channels 5-8 Frequency Spectrum')
        plt.xlabel('Frequency(Hz)')
        plt.ylabel('Power')
        plt.plot(f, CH5, alpha=0.9)
        plt.plot(f, CH6, alpha=0.7)
        plt.plot(f, CH7, alpha=0.5)
        plt.plot(f,CH8, alpha=0.3)
        plt.legend(['Ch5', 'Ch6', 'Ch7', 'Ch8'], loc=4)
        plt.xlim(0, 20)

        for i in range(1, 30):
            plt.axvline(x=1 * i, linewidth=0.2, color='grey')

    # small subplots
    p = 0
    sigNois = [1.1,2.2,4.4,4.4,5.5,5.4]
    for ch in sensor:
        dataT = sensor[p]

        if sampRate == 256:
            t = np.arange(startT, endTim -(winsize*(1 / sampRate))+(1 / sampRate), 1 / sampRate)
        else:
            t = np.arange(startT, endTim, 1 / sampRate)
        dataT = ((dataT.T - np.mean(dataT, axis=0)) / np.std(dataT, axis=0)).T
        dataT = filter_data(dataT, sampRate, 3, 6, method='iir')

        #right here add a signal to noise ratio part
        power = np.log(dataT ** 2 + 0.01)
        avgPow = np.mean(power)#cont tomo
        sigNois[p] = avgPow


        z = hilbert(dataT)  # form the analytical signal
        inst_amplitude = np.abs(z)  # envelope extraction
        inst_phase = np.unwrap(np.angle(z))  # inst phase
        inst_freq = (np.diff(inst_phase) / (
                2.0 * np.pi) * sampRate)  # inst frequency = the derivative of instantaneous phase.
                # when the amplitude is really small, we get a high or low frequency peak since it is not clear what frequency the signal has
                # therefore we select the indices of these very small amplitudes and make sure that we than take the average frequency signal

        locbig = np.nonzero(inst_amplitude[:-1] < 0.2)
        inst_freq[locbig] = np.nan
                # The instantaneous temporal frequency is derived as f(t)=0.5/π*d/dt*ϕ(t)
                # The instantaneous angular frequency is derived as ω(t)=d/dt*ϕ(t)
                # kijken naar instantane fase.
                # verandering in frequentie over tijd.
                # angle van hilbert pakken en dit differentieren als maat voor frequentie

        plt.subplot2grid((4, len(sensor)), (2, p))
        plt.xlabel('Time(s)')
        plt.ylabel('Amplitude 3-6 Hz signal')
        plt.plot(t, power, label='power')
        plt.plot(t, inst_amplitude, label='envelope')
        plt.legend()
        plt.ylim(0.0, 5.0)

        plt.subplot2grid((4, len(sensor)), (3, p))
        plt.xlabel('Time(s)')
        plt.ylabel('Frequency(Hz)')
        plt.plot(t[:-1], inst_freq)
        plt.ylim(0.0, 15.0)

        # plt.tight_layout()
        p += 1

    #plt.savefig(fname="graph " + str(sampR) + " - " +str(count) + " TR03")
    maxPo = sigNois.index(max(sigNois))
    sumAvg = 0
    l = 0
    for l in sigNois:
        sumAvg += sigNois.index(l)

    sumAvg = sumAvg - maxPo
    sumAvg = sumAvg/(len(sigNois)-1)

    s2n = (maxPo - sumAvg)/sumAvg
    saveName = "graph" + str(count)
    plt.savefig(saveName)
    #plt.show()
            # plt.show()#change to plt. save or something
            # also need to use scipy.stats.signaltonoise for signal to noise ratio of each channel



x = 0
while x < numTremors: #loop to make tremors
    q = 0
    while q < len(holdstrings):
        a = edf.EdfReader(holdstrings[q])
        sampR = a.getSampleFrequency(0)

        if x == 0:
            sig1, sig2 = getsignaltonoise(a)

        if sampR == 200:
            startTime = float(dataG[x][0])
            endTime = float(dataG[x][1])
        elif sampR == 256:
            startTime = float(dataE[x][0])
            endTime = float(dataE[x][1])
        else:
            startTime = float(dataE[x][0])
            endTime = float(dataE[x][1])
            sampR = 256

        graphtremor(startTime, endTime, a, sampR, q, newoffset)
        q = q + 1

    x = x + 1






