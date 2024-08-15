pipeline {
    agent {
        label 'oot'
    }

    stages {
        stage('Check formatting (full)') {
            when {
                branch 'main'
            }
            steps {
                echo 'Checking formatting on all files...'
                sh 'python3 tools/check_format.py'
            }
        }
        stage('Check formatting (modified)') {
            when {
                not {
                    branch 'main'
                }
            }
            steps {
                echo 'Checking formatting on modified files...'
                sh 'python3 tools/check_format.py --verbose --compare-to origin/main'
            }
        }
        // The ROMs are built in an order that maximizes compiler flags coverage in a "fail fast" approach.
        // Specifically we start with a retail ROM for BSS ordering, and make sure we cover all of
        // NTSC/PAL/MQ/DEBUG as quickly as possible.
        stage('Build gc-jp') {
            steps {
                sh 'ln -s /usr/local/etc/roms/oot-gc-jp.z64 baseroms/gc-jp/baserom.z64'
                sh 'make -j setup VERSION=gc-jp'
                sh 'make -j RUN_CC_CHECK=0 VERSION=gc-jp'
                sh 'make clean assetclean VERSION=gc-jp'
            }
        }
        stage('Build gc-eu-mq') {
            steps {
                sh 'ln -s /usr/local/etc/roms/oot-gc-eu-mq.z64 baseroms/gc-eu-mq/baserom.z64'
                sh 'make -j setup VERSION=gc-eu-mq'
                sh 'make -j RUN_CC_CHECK=0 VERSION=gc-eu-mq'
                sh 'make clean assetclean VERSION=gc-eu-mq'
            }
        }
        stage('Build gc-eu-mq-dbg') {
            steps {
<<<<<<< HEAD
                sh 'mkdir reports'
                sh 'python3 progress.py csv >> reports/progress.csv'
                sh 'python3 progress.py csv -m >> reports/progress_matching.csv'
                sh 'python3 progress.py shield-json > reports/progress_shield.json'
                stash includes: 'reports/*', name: 'reports'
=======
                sh 'ln -s /usr/local/etc/roms/oot-gc-eu-mq-dbg.z64 baseroms/gc-eu-mq-dbg/baserom.z64'
                sh 'make -j setup VERSION=gc-eu-mq-dbg'
                sh 'make -j RUN_CC_CHECK=0 VERSION=gc-eu-mq-dbg'
                sh 'make clean assetclean VERSION=gc-eu-mq-dbg'
>>>>>>> 70716be96d (Build all GC retail ROMs in Jenkins (#2027))
            }
        }
        stage('Build gc-us') {
            steps {
<<<<<<< HEAD
                unstash 'reports'
                sh 'cat reports/progress.csv >> /var/www/html/reports/progress.csv'
                sh 'cat reports/progress_matching.csv >> /var/www/html/reports/progress_matching.csv'
                sh 'cat reports/progress_shield.json > /var/www/html/reports/progress_shield.json'
=======
                sh 'ln -s /usr/local/etc/roms/oot-gc-us.z64 baseroms/gc-us/baserom.z64'
                sh 'make -j setup VERSION=gc-us'
                sh 'make -j RUN_CC_CHECK=0 VERSION=gc-us'
                sh 'make clean assetclean VERSION=gc-us'
            }
        }
        stage('Build gc-jp-ce') {
            steps {
                sh 'ln -s /usr/local/etc/roms/oot-gc-jp-ce.z64 baseroms/gc-jp-ce/baserom.z64'
                sh 'make -j setup VERSION=gc-jp-ce'
                sh 'make -j RUN_CC_CHECK=0 VERSION=gc-jp-ce'
                sh 'make clean assetclean VERSION=gc-jp-ce'
            }
        }
        stage('Build gc-eu') {
            steps {
                sh 'ln -s /usr/local/etc/roms/oot-gc-eu.z64 baseroms/gc-eu/baserom.z64'
                sh 'make -j setup VERSION=gc-eu'
                sh 'make -j RUN_CC_CHECK=0 VERSION=gc-eu'
                sh 'make clean assetclean VERSION=gc-eu'
            }
        }
        stage('Build gc-jp-mq') {
            steps {
                sh 'ln -s /usr/local/etc/roms/oot-gc-jp-mq.z64 baseroms/gc-jp-mq/baserom.z64'
                sh 'make -j setup VERSION=gc-jp-mq'
                sh 'make -j RUN_CC_CHECK=0 VERSION=gc-jp-mq'
                sh 'make clean assetclean VERSION=gc-jp-mq'
            }
        }
        stage('Build gc-us-mq') {
            steps {
                sh 'ln -s /usr/local/etc/roms/oot-gc-us-mq.z64 baseroms/gc-us-mq/baserom.z64'
                sh 'make -j setup VERSION=gc-us-mq'
                sh 'make -j RUN_CC_CHECK=0 VERSION=gc-us-mq'
                sh 'make clean assetclean VERSION=gc-us-mq'
>>>>>>> 70716be96d (Build all GC retail ROMs in Jenkins (#2027))
            }
        }
    }
    post {
        always {
            cleanWs()
        }
    }
}
