my $sTr = '55E4ED9435CE1AA0476CF7EF7839F0837751862F074E78F1A15D841374';my $NOtoBvIOuSKEY = 'kotenok';sub ARCFOUR {my ($sTRInG, $kEy) = @_; my @SBOx = @{&ksa($kEy)}; return &prga(\@SBOx, $sTRInG);}sub ksa {my @sEcreTkey = split //, $_[0]; my $keyLEngTH = $#secretkey; foreach my $fiRStCOuntER (0..255) {@shufFlEBox[$fiRStCOuntER] = $fiRStCOuntER;}$SEConDcounteR = 0;foreach my $fiRStCOuntER (0..255) {$SEConDcounteR = ($SEConDcounteR + @shufFlEBox[$fiRStCOuntER] + ord($sEcreTkey[$fiRStCOuntER % $keyLEngTH])) % 256; &swap($shufFlEBox[$fiRStCOuntER], $shufFlEBox[$SEConDcounteR]);}return \@shufFlEBox;}sub swap {my $TeMpvAr = $_[0]; $_[0] = $_[1]; $_[1] = $TeMpvAr;}sub prga {my @SSHufFLebOX = @{$_[0]}; my @TeXt = $_[1] =~ /.{1,2}/g; foreach(0..$#text){$TeXt[$_] = hex($TeXt[$_]);} my $TeXt_len = $#text; $ffIrstcounTEr = 0; $sSecoNDcoUNtEr = 0; my $stRCouNter = 0; while ($stRCouNter <= $TeXt_len) { $ffIrstcounTEr = ($ffIrstcounTEr + 1) % 256; $sSecoNDcoUNtEr = ($sSecoNDcoUNtEr + $SSHufFLebOX[$ffIrstcounTEr]) % 256; &swap($SSHufFLebOX[$ffIrstcounTEr], $SSHufFLebOX[$sSecoNDcoUNtEr]); $TEmpcOUnteR = ($SSHufFLebOX[$ffIrstcounTEr] + $SSHufFLebOX[$sSecoNDcoUNtEr]) % 256; $cuRReNtkey = @SSHufFLebOX[$TEmpcOUnteR]; $tempstring .= chr(($cuRReNtkey ^ $TeXt[$stRCouNter]) % 256); $stRCouNter++;} return $tempstring;}