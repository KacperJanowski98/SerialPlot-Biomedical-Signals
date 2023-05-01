import re
import os
import sys
from pathlib import Path
from heartpy import heartpy, analysis
import numpy


class HeartAnalysis:
    """
    Heart rate analysis based on PPG or ECG signal.
    """
    def __init__(self, sampling: float, column_name: str):
        self.err_status = 0
        self.sampling = sampling
        self.dir_path = os.path.dirname(__file__)
        self.data = heartpy.get_data(filename=f"{self.dir_path}/data.csv", delim=',', column_name=column_name)
        try:
            self.working_data, self.measures = heartpy.process(self.data, self.sampling)
            self.err_msg = ""
            self.flag = True
        except heartpy.exceptions.BadSignalWarning as err:
            self.err_msg = str(err)
            self.flag = False

    def get_err_message(self) -> str:
        """
        Get error information when performing an analysis.
        :return: Error information.
        """
        return self.err_msg
    
    def get_err_status(self) -> int:
        """
        Get error status.
        :return: Error status.
        """
        return self.err_status

    def get_bpm(self) -> float:
        """
        Heart rate download - the number of heart beats per minute.
        :return: Heart rate.
        """
        if self.flag:
            if numpy.isnan(self.measures['bpm']):
                self.err_status = 1
                return float(0.0)
            else:
                self.err_status = 0
                return round(self.measures['bpm'], 3) if type(self.measures['bpm']) == numpy.float64 else float(0.0)
        else: 
            self.err_status = 1
            return float(0.0)

    def get_ibi(self) -> float:
        """
        The interval between heartbeats.
        :return: RR interval.
        """
        if self.flag:
            if numpy.isnan(self.measures['ibi']):
                self.err_status = 1
                return float(0.0)
            else:
                self.err_status = 0
                return round(self.measures['ibi'], 3) if type(self.measures['ibi']) == numpy.float64 else float(0.0)
        else: 
            self.err_status = 1
            return float(0.0)

    def get_sdnn(self) -> float:
        """
        Standard deviation of RR intervals.
        :return: Deviation.
        """
        if self.flag:
            if numpy.isnan(self.measures['sdnn']):
                self.err_status = 1
                return float(0.0)
            else:
                self.err_status = 0
                return round(self.measures['sdnn'], 3) if type(self.measures['sdnn']) == numpy.float64 else float(0.0)
        else: 
            self.err_status = 1
            return float(0.0)

    def get_sdsd(self) -> float:
        """
        Standard deviation of successive differences.
        :return: Deviation.
        """
        if self.flag:
            if numpy.isnan(self.measures['sdsd']):
                self.err_status = 1
                return float(0.0)
            else:
                self.err_status = 0
                return round(self.measures['sdsd'], 3) if type(self.measures['sdsd']) == numpy.float64 else float(0.0)
        else: 
            self.err_status = 1
            return float(0.0)

    def get_rmssd(self) -> float:
        """
        RMS of consecutive differences.
        :return: RMS.
        """
        if self.flag:
            if numpy.isnan(self.measures['rmssd']):
                self.err_status = 1
                return float(0.0)
            else:
                self.err_status = 0
                return round(self.measures['rmssd'], 3) if type(self.measures['rmssd']) == numpy.float64 else float(0.0)
        else: 
            self.err_status = 1
            return float(0.0)

    def get_hr_mad(self) -> float:
        """
        Median absolute deviation of RR intervals.
        :return: Median deviation.
        """
        if self.flag:
            if numpy.isnan(self.measures['hr_mad']):
                self.err_status = 1
                return float(0.0)
            else:
                self.err_status = 0
                return round(self.measures['hr_mad'], 3) if type(self.measures['hr_mad']) == numpy.float64 else float(0.0)
        else: 
            self.err_status = 1
            return float(0.0)

    def get_breathingrate(self) -> float:
        """
        Respiratory rate.
        :return: Breathingrate.
        """
        if self.flag:
            if numpy.isnan(self.measures['breathingrate']):
                self.err_status = 1
                return float(0.0)
            else:
                self.err_status = 0
                return round(self.measures['breathingrate'], 3) if type(self.measures['breathingrate']) == numpy.float64 else float(0.0)
        else: 
            self.err_status = 1
            return float(0.0)
