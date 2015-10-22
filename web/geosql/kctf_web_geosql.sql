SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";

CREATE TABLE IF NOT EXISTS `notes` (
  `id` int(11) NOT NULL,
  `position` point NOT NULL,
  `text` varchar(32) COLLATE utf8_bin NOT NULL,
  `secret` varchar(25) COLLATE utf8_bin NOT NULL
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

ALTER TABLE `notes` ADD PRIMARY KEY (`id`);
ALTER TABLE `notes`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=1;

INSERT INTO `notes` (`id`, `position`, `text`, `secret`) VALUES
(16, GeomFromText('POINT(55.994510929106866 92.7975869178772)'), 'Secret here', 'flag is:#880055535350');

