import re
import os
import sys
from pathlib import Path
from heartpy import heartpy, analysis

def print_something():
    print("Hello from Python!")

def calc_basic() -> float:
    dir_path = os.path.dirname(__file__)
    # print(dir_path)
    # project_dir = Path(__file__).parent.parent.parent
    # print(project_dir)
    # print(sys.path[1])
    hrdata = heartpy.get_data(filename=f"{dir_path}/A0001.csv", delim=',', column_name='Basic_signal')
    working_data, measures = heartpy.process(hrdata, 300.0)
    measures, working_data = analysis.calc_breathing(rrlist=working_data['RR_list_cor'],
                                                        measures=measures, working_data=working_data)
    # print(f"Basic signal breathing rate: {round(measures['breathingrate'], 3)} Hz")
    return measures['breathingrate']
