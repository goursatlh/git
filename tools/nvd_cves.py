#!/usr/bin/env python3
# -*- coding: utf-8 -*- 
import re, requests, os, sys, getopt
scrVers = '1.55'

def get_single(argv):
    fcnName = 'get_single'
    helpTxt = """
{scrName}

Function {fcnName}
  USAGE
    Display this usage information.
      {fcnName}(help=1)
  USAGE
    Display this usage information.
      {scrName} -h

    Display script version information.
      {scrName} -V

    Download vulnerability CVEs from https://nvd.nist.gov for component and version.
      {scrName} [OPTIONS] -c <component> -v <version>

  OPTIONS
    Quiet mode - execute without outputting except on error
    -q

    Verbose mode (ignored when in quiet mode)
    --verbose

    Limit the request to N number of pages or queries for the component
    --max-numpages N
      where N is an integer

  EXAMPLE
    <path to nvd_cves.py> -c apple -v 1.2.2 --verbose --max-numpages 4
    or (executes same request)
    python -c "import sys; sys.path.insert(1, r'%UserProfile%/OneDrive - Vantiva/Documents/new role tools/tools'); import nvd_cves; nvd_cves.get_single(('-c', 'apple', '-v', '1.2.2', '--verbose', '--max-numpages', '3'))"

  EXIT
    Exits with a code of 0 when successful.
    Exits with a code of 1 if error accessing URL.
    Exits with a code of 2 if system errors encountered.
    Exits with a code of 3 if no records could be found at https://nvd.nist.gov for
    the given component and version.
    Exits with a code of 4 if executed improperly.
    Exits with a code of 5 for database error.
""".format(scrName=os.path.basename(__file__),fcnName=fcnName)

    component = ''
    version = ''
    quietMode = 0
    verboseMode = 0
    maxPages = 0
    try:
        opts, args = getopt.getopt(argv,"qhVc:v:",["help","component=","comp-vers=","max-numpages=","verbose"])
    except getopt.GetoptError:
        print('Usage error! Use --help argument for usage information.', file=sys.stderr)
        print(helpTxt, file=sys.stderr)
        return 4
        #sys.exit(4)
    print(opts)
    for opt, arg in opts:
        if opt in ("-h", "--help"):
            print(helpTxt)
            #get_batch("-c {} -v {}{}".format(pair[0], pair[1], extra).split())
            return 0
            #sys.exit(0)
        elif opt == '-V':
            print('{name}\tVers. {vers}\n'.format(name=os.path.basename(__file__), vers=scrVers))
            return 0
            #sys.exit(0)
        elif opt == '-q':
            quietMode = 1
        elif opt in ("-c", "--component"):
            component = arg
        elif opt in ("--max-numpages"):
            try:
                maxPages = int(arg)
                break
            except ValueError:
                print("Error!  Invalid integer specified for --max-numpages.")
                return 4
        elif opt in ("-v", "--comp-vers"):
            version = arg
        elif opt == '--verbose':
            verboseMode = 1

    if not component or not version:
        print('Usage error! Use --help argument for usage information.', file=sys.stderr)
        return 4
        #sys.exit(4)

    if quietMode:
        verboseMode = 0
    
    if verboseMode:
        print("Component = '{component}', Version = {version}".format(component=component, version=version))
    if not quietMode:
        print('Please be patient while your files are downloaded.')

    # Automatically set the CPE part portion of the URL. 'linux_kernel' is an operating system.
    if component == 'linux_kernel':
        cpe_part = 'o'  # operating system
    else:
        cpe_part = 'a'  # application
    url = 'https://nvd.nist.gov/vuln/search/results?query=cpe%3A2.3%3A' + cpe_part + '%3A*%3A' +\
        component + '%3A' + version + '&results_type=overview&form_type=Advanced&search_type=all' +\
        '&isCpeNameSearch=false'

    # lastSeq is the max number of records we need to request (where
    # there are up to 20 records per request).
    # seq is the starting record number for the current request iteration.
    # Start off with seq 0, and make sure we at least go through one iteration.
    count = 1  # for limiting the number of downloaded pages
    lastSeq = 20
    seq = 0
    while (seq < lastSeq):
        currUrl = url + '&startIndex=' + str(seq)
        if not quietMode:
            print('Downloading "' + currUrl + '"')
        # Download the HTML data file
        html = requests.get(currUrl, allow_redirects=True)
        component = re.sub(r'\*', '~', component)
        #if (version == "*"):
        #    resultFile = 'NVD - {component} ~ {sequence}.html'.format(
        #        component=component, sequence=seq)
        #    resultFileNone = 'NVD - {component} ~ {sequence}.none.html'.format(
        #        component=component, sequence=seq)
        #else:
        version = re.sub(r'\*', '~', version)
        resultFile = 'NVD - {component} {version} {sequence}.html'.format(
            component=component, version=version, sequence=seq)
        resultFileNone = 'NVD - {component} {version} {sequence}.none.html'.format(
            component=component, version=version, sequence=seq)
        try:
            open(resultFile, 'wb').write(html.content)
        except OSError:
            print('Error creating/writing file "{}" or getting data from https://nvd.nist.gov'.format(resultFile), file=sys.stderr)
            return 2
            #sys.exit(2)
        if seq == 0:
            # Save data for later
            data = str(html.content)

            if verboseMode:
                print('Examining HTML file: {}'.format(resultFile))
            if (data.count("<title>NVD - Timeout Error</title>")):
                resultFileTimeout = 'NVD - {component} {version} {sequence}.timeout.html'.format(
                    component=component, version=version, sequence=seq)
                if not quietMode:
                    print('Database timeout for component="{component}" and version="{version}"'.format(component=component, version=version), file=sys.stderr)
                os.rename(resultFile, resultFileTimeout)
                return 5
            maxRecs = re.search(r"There are <strong data-testid=\"vuln-matching-records-count\">(\d+)</strong> matching records\.",
                                data).group(1)
            if (int(maxRecs) == 0):
                # No records found in the data string
                if not quietMode:
                    print('No records found for component="{component}" and version="{version}"'.format(component=component, version=version), file=sys.stderr)
                os.rename(resultFile, resultFileNone)
                return 3
                #sys.exit(3)

            if not quietMode:
                print('Total number of records found: {}'.format(maxRecs))
            # Compute the total number of iterations (seqs) we need in order to retrieve all data.
            lastSeq = int(maxRecs)
        seq += 20
        if maxPages > 0 and count >= maxPages:
            return 0
        count += 1
    return 0


def get_batch(*argv, **kwargs):
    # To run this function from the command line we can use the -c (command) argument as follows:
    # $ python -c "import nvd_cves; nvd_cves.get_batch('')"
    # Or for a single component:
    # $ python -c "import sys; sys.path.insert(1, '<path to nvd_cves.py folder>'); import nvd_cves; nvd_cves.get_single(('-c', 'apple', '-v', '1.2.2', '--verbose', '--max-numpages', '4'))"
    fcnName = 'get_batch'
    helpTxt = """
Function {fcnName}
  USAGE
    Display this usage information.
      {fcnName}(help=1)

    Download vulnerability CVEs from https://nvd.nist.gov for component and version.
      python -c "import nvd_cves; nvd_cves.{fcnName}(<component-version pairs list>[, [OPTIONS]])"
        where:
            <component-version pairs list> is a quoted comma-separated list of component-version
            pairs, where components and versions are separated by a space character.
            Example:
                python -c "import nvd_cves; nvd_cves.get_batch('apple 1.2.2, pear 2.9a, orange 3.2.3', verbose=1)"
        You may use the '*' wildcard for the version (ot obtain CVEs against all versions of the given component.
        You may also use the '*' wildcard at the beginning and/or end of the component name. In such cases, the
        asterisk will be replaced by '~' in the resultant filename.
        
    If this python module is not in the current directory, you may use the following command syntax:
      python -c "import sys; sys.path.insert(1, '<folder path containing module>'); import nvd_cves; nvd_cves.get_batch('apple 1.2.2, pear 2.9a, orange 3.2.3', verbose=1)"
    Examples:
      python -c "import sys; sys.path.insert(1, r'%UserProfile%/OneDrive - Vantiva/Documents/new role tools/tools'); import nvd_cves; nvd_cves.get_batch('apple 1.2.2, pear 2.9a, orange 3.2.3', verbose=1)"
      python -c "import sys; sys.path.insert(1, r'%UserProfile%/OneDrive - Vantiva/Documents/new role tools/tools'); import nvd_cves; nvd_cves.get_batch('apple 1.2.2, pear 2.9a, orange 3.2.3', verbose=1, maxnumpages=2, keep_only_successful=1)"
    Or for a single module:
      python nvd_cves.py -c linux_kernel -v 5.15.77 --max-numpages 4

  OPTIONS (comma-separated list of keyword=value pairs)
    Quiet mode - execute without outputting except on error
    quiet=1

    Verbose mode (ignored when in quiet mode)
    verbose=1

    Limit the request to N number of pages or queries for a component
    maxnumpages=N
      where N is an integer

Keep only downloaded files with reported vulnerabilities
    keep_only_successful=1

  EXIT
    Exits with a code of 0 when successful.
    Exits with a code of 1 if error accessing URL.
    Exits with a code of 2 if system errors encountered.
    Exits with a code of 4 if executed improperly.
    Exits with a code of 5 for database error.
""".format(fcnName=fcnName)

    component = ''
    version = ''
    quietMode = 0
    verboseMode = 0
    deleteZeroReported = 0
    maxNumPages = 0

    if not len(argv):
        print(helpTxt)
        sys.exit(0)

    # Handle options (keyword-value)
    for param, value in kwargs.items():
        if param.lower() == 'help':
            if value == 1:
                print("USE CASE: batch")
                print(helpTxt)
                sys.exit(0)
        elif param.lower() == 'quiet':
            if value == 1:
                quietMode = 1
        elif param.lower() == 'verbose':
            if value == 1:
                verboseMode = 1
        elif param.lower() == 'maxnumpages':
            print('got maxnumpages')
            try:
                maxNumPages = int(value)
            except ValueError:
                print("Error!  Invalid integer specified for maxnumpages.")
                sys.exit(4)
        elif param.lower() == 'keep_only_successful':
            if value == 1:
                deleteZeroReported = 1

    extra = ''
    if quietMode:
        verboseMode = 0
        extra = ' -q'
    if verboseMode:
        extra = extra + ' --verbose'
    if maxNumPages != 0:
        extra = extra + ' --max-numpages ' + str(maxNumPages)

    args = argv[0]
    mylist = args.split(',')

    for couple in mylist:
        couple = couple.strip()
        couple = re.sub(' +', ' ', couple)
        pair = couple.split(' ')
        result = get_single("-c {} -v {}{}".format(pair[0], pair[1], extra).split())
        if ((result != 3) and (result != 0)):
            print("Result is {}".format(result))
            # errors for which we exit
            sys.exit(result)
        elif deleteZeroReported == 1 and result == 3:
            if (pair[1] == "*"):
                resultFile = 'NVD - {component} - {sequence}.none.html'.format(
                    component=pair[0], sequence=0)
            else:
                resultFile = 'NVD - {component} {version} {sequence}.none.html'.format(
                    component=pair[0], version=pair[1], sequence=0)
            if os.path.exists(resultFile):
                os.remove(resultFile)
            if not quietMode:
                print("Deleting {}".format(resultFile))
    sys.exit(result)

if __name__ == "__main__":
    ret = get_single(sys.argv[1:])
    if (len(sys.argv[1:]) == 1):
        if (sys.argv[1] == '-h' or sys.argv[1] == '--help'):
            get_batch()
    sys.exit(ret)
