import pandas as pd
import matplotlib.pyplot as plt
plt.style.use('ggplot')

HIST_FILE = pd.read_csv('HIST_FILE.txt', sep=" ", header=None)

SORTED_HIST_FILE = HIST_FILE.sort_values(by=[3])

plt.hist(SORTED_HIST_FILE[3])
plt.title('Distribution of Flow Counts')
plt.xlabel('Number of Flows')
plt.ylabel('Frequency of Flows')

plt.savefig('hist.png')
